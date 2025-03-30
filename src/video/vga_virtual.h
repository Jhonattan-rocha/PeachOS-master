#pragma once

#include <stdint.h>
#include <stddef.h>

#define VGA_HW_WIDTH 80
#define VGA_HW_HEIGHT 25
#define VGA_MEM_ADDR 0xB8000

#define VGA_VIRTUAL_WIDTH 120
#define VGA_VIRTUAL_HEIGHT 100

// expor a variavel para qualquer parte do sistema
extern int cursor_x;
extern int cursor_y;

typedef struct {
    char character;
    char color;
} vga_cell_t;

void vga_virtual_init();
void vga_virtual_clear();
void vga_virtual_put_char(int x, int y, char c, char color);
void vga_virtual_write(const char* str, char color);
void vga_virtual_scroll(int lines);
void vga_virtual_flush_viewport(int vx, int vy);
void vga_virtual_set_cursor(int x, int y);
