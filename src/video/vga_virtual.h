#pragma once

#include <stdint.h>
#include <stddef.h>

#define VGA_HW_WIDTH 80
#define VGA_HW_HEIGHT 25
#define VGA_MEM_ADDR 0xB8000

#define VGA_VIRTUAL_WIDTH 120
#define VGA_VIRTUAL_HEIGHT 100

typedef struct {
    int cursor_x;
    int cursor_y;
    int viewport_x;
    int viewport_y;
    uint16_t* vga_mem;
    uint16_t backbuffer[VGA_VIRTUAL_WIDTH * VGA_VIRTUAL_HEIGHT];
} vga_virtual_context_t;

void vga_virtual_init(vga_virtual_context_t* ctx);
void vga_virtual_clear(vga_virtual_context_t* ctx);
void vga_virtual_write(vga_virtual_context_t* ctx, const char* str, uint8_t color);
void vga_virtual_put_char(vga_virtual_context_t* ctx, int x, int y, char c, uint8_t color);
void vga_virtual_set_cursor(vga_virtual_context_t* ctx, int x, int y);
void vga_virtual_scroll_viewport(vga_virtual_context_t* ctx, int lines);
void vga_virtual_flush(vga_virtual_context_t* ctx);
