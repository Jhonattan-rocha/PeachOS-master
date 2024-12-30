#include "classic.h"
#include "keyboard.h"
#include "io/io.h"
#include "kernel.h"
#include "idt/idt.h"
#include "task/task.h"
#include "string/string.h"

#include <stdint.h>
#include <stddef.h>

#define CLASSIC_KEYBOARD_CAPSLOCK 0x3A
static bool is_shift_pressed = false;

int classic_keyboard_init();

static uint8_t keyboard_scan_set_one[] = {
    0x00, 0x1B, '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '-', '=',
    0x08, '\t', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '[', ']',
    0x0d, 0x00, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ';', '\'', '`', 
    0x00, '\\', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', ',', '.', '/', 0x00, '*',
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0', '.'
};

static uint8_t keyboard_scan_set_one_with_shift[] = {
    0x00, 0x1B, '!', '@', '#', '$', '%',
    '^', '&', '*', '(', ')', '_', '+',
    0x08, '\t', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '{', '}',
    0x0d, 0x00, 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ':', '"', '~', 
    0x00, '|', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', '<', '>', '?', 0x00, '*',
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, '7', '8', '9', '-', '4', '5',
    '6', '+', '1', '2', '3', '0', '.'
};

struct keyboard classic_keyboard = {
    .name = {"Classic"},
    .init = classic_keyboard_init
};


void classic_keyboard_handle_interrupt();

int classic_keyboard_init()
{
    idt_register_interrupt_callback(ISR_KEYBOARD_INTERRUPT, classic_keyboard_handle_interrupt);

    keyboard_set_capslock(&classic_keyboard, KEYBOARD_CAPS_LOCK_OFF);

    outb(PS2_PORT, PS2_COMMAND_ENABLE_FIRST_PORT);
    return 0;
}

uint8_t classic_keyboard_scancode_to_char(uint8_t scancode)
{
    size_t size_of_keyboard_set_one = sizeof(keyboard_scan_set_one) / sizeof(uint8_t);
    if (scancode >= size_of_keyboard_set_one)
    {
        return 0;
    }

    char c = is_shift_pressed
        ? keyboard_scan_set_one_with_shift[scancode]
        : keyboard_scan_set_one[scancode];
    
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        c += 32;
    }

    // Verifica o estado do Caps Lock
    if (keyboard_get_capslock(&classic_keyboard) == KEYBOARD_CAPS_LOCK_ON)
    {
        if (c >= 'a' && c <= 'z') // Minúscula para maiúscula
        {
            c -= 32;
        }
        else if (c >= 'A' && c <= 'Z') // Maiúscula para minúscula
        {
            c += 32;
        }
    }

    // Aplica o Shift invertendo novamente se necessário
    if (is_shift_pressed)
    {
        if (c >= 'a' && c <= 'z') // Minúscula para maiúscula
        {
            c -= 32;
        }
        else if (c >= 'A' && c <= 'Z') // Maiúscula para minúscula
        {
            c += 32;
        }
    }

    return c;
}

void classic_keyboard_handle_interrupt()
{
    kernel_page();
    uint8_t scancode = insb(KEYBOARD_INPUT_PORT);
    insb(KEYBOARD_INPUT_PORT); // Lê novamente para limpar a fila

    // Verifica se o scancode é de Shift
    if ((scancode & 0x7F) == CLASSIC_KEYBOARD_LEFT_SHIFT || 
        (scancode & 0x7F) == CLASSIC_KEYBOARD_RIGHT_SHIFT)
    {
        is_shift_pressed = !(scancode & CLASSIC_KEYBOARD_KEY_RELEASED);
        return;
    }
    
    // Ignora teclas liberadas
    if (scancode & CLASSIC_KEYBOARD_KEY_RELEASED)
    {
        return;
    }

    // Verifica o Caps Lock
    if (scancode == CLASSIC_KEYBOARD_CAPSLOCK)
    {
        KEYBOARD_CAPS_LOCK_STATE old_state = keyboard_get_capslock(&classic_keyboard);
        keyboard_set_capslock(&classic_keyboard, old_state == KEYBOARD_CAPS_LOCK_ON
            ? KEYBOARD_CAPS_LOCK_OFF 
            : KEYBOARD_CAPS_LOCK_ON);
        return;
    }

    // Converte o scancode para caractere
    uint8_t c = classic_keyboard_scancode_to_char(scancode);

    // Adiciona o caractere ao buffer, se válido
    if (c != 0)
    {
        keyboard_push(c);
    }

    task_page();
}


struct keyboard* classic_init()
{
    return &classic_keyboard;
}