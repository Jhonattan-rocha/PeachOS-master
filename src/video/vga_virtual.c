#include "video/vga_virtual.h"
#include "string/string.h"

static vga_cell_t vga_backbuffer[VGA_VIRTUAL_WIDTH * VGA_VIRTUAL_HEIGHT];
static uint16_t* vga_mem = (uint16_t*)VGA_MEM_ADDR;

static int viewport_x = 0;
static int viewport_y = 0;

int cursor_x = 0;
int cursor_y = 0;

static uint16_t vga_make_entry(char c, char color) {
    return (color << 8) | c;
}

void vga_virtual_init() {
    vga_virtual_clear();
    vga_virtual_flush_viewport(0, 0);
}

void vga_virtual_clear() {
    for (int y = 0; y < VGA_VIRTUAL_HEIGHT; y++) {
        for (int x = 0; x < VGA_VIRTUAL_WIDTH; x++) {
            vga_backbuffer[y * VGA_VIRTUAL_WIDTH + x].character = ' ';
            vga_backbuffer[y * VGA_VIRTUAL_WIDTH + x].color = 0x07;
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_virtual_put_char(int x, int y, char c, char color) {
    if (x < 0 || x >= VGA_VIRTUAL_WIDTH || y < 0 || y >= VGA_VIRTUAL_HEIGHT)
        return;

    vga_backbuffer[y * VGA_VIRTUAL_WIDTH + x].character = c;
    vga_backbuffer[y * VGA_VIRTUAL_WIDTH + x].color = color;
}

void vga_virtual_write(const char* str, char color) {
    for (size_t i = 0; str[i]; i++) {
        if (str[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
            continue;
        }
        vga_virtual_put_char(cursor_x, cursor_y, str[i], color);
        cursor_x++;
        if (cursor_x >= VGA_VIRTUAL_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
}

void vga_virtual_scroll(int lines) {
    viewport_y += lines;
    if (viewport_y < 0) viewport_y = 0;
    if (viewport_y > VGA_VIRTUAL_HEIGHT - VGA_HW_HEIGHT)
        viewport_y = VGA_VIRTUAL_HEIGHT - VGA_HW_HEIGHT;
    vga_virtual_flush_viewport(viewport_x, viewport_y);
}

void vga_virtual_flush_viewport(int vx, int vy) {
    viewport_x = vx;
    viewport_y = vy;

    for (int y = 0; y < VGA_HW_HEIGHT; y++) {
        for (int x = 0; x < VGA_HW_WIDTH; x++) {
            int virtual_x = vx + x;
            int virtual_y = vy + y;

            if (virtual_x >= VGA_VIRTUAL_WIDTH || virtual_y >= VGA_VIRTUAL_HEIGHT) {
                vga_mem[y * VGA_HW_WIDTH + x] = vga_make_entry(' ', 0x07);
            } else {
                vga_cell_t cell = vga_backbuffer[virtual_y * VGA_VIRTUAL_WIDTH + virtual_x];
                vga_mem[y * VGA_HW_WIDTH + x] = vga_make_entry(cell.character, cell.color);
            }
        }
    }
}

void vga_virtual_set_cursor(int x, int y) {
    cursor_x = x;
    cursor_y = y;
}
