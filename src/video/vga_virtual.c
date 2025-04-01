#include "video/vga_virtual.h"
#include "string/string.h"

static uint16_t vga_make_entry(char c, uint8_t color) {
    return (color << 8) | (uint8_t)c;
}

void vga_virtual_init(vga_virtual_context_t* ctx) {
    ctx->cursor_x = 0;
    ctx->cursor_y = 0;
    ctx->viewport_x = 0;
    ctx->viewport_y = 0;
    ctx->vga_mem = (uint16_t*)VGA_MEM_ADDR;
    vga_virtual_clear(ctx);
    vga_virtual_flush(ctx);
}

void vga_virtual_clear(vga_virtual_context_t* ctx) {
    for (int y = 0; y < VGA_VIRTUAL_HEIGHT; y++) {
        for (int x = 0; x < VGA_VIRTUAL_WIDTH; x++) {
            ctx->backbuffer[y * VGA_VIRTUAL_WIDTH + x] = vga_make_entry(' ', 0x07);
        }
    }
    ctx->cursor_x = 0;
    ctx->cursor_y = 0;
}

void vga_virtual_set_cursor(vga_virtual_context_t* ctx, int x, int y) {
    ctx->cursor_x = x;
    ctx->cursor_y = y;
}

void vga_virtual_put_char(vga_virtual_context_t* ctx, int x, int y, char c, uint8_t color) {
    if (x < 0 || x >= VGA_VIRTUAL_WIDTH || y < 0 || y >= VGA_VIRTUAL_HEIGHT)
        return;
    ctx->backbuffer[y * VGA_VIRTUAL_WIDTH + x] = vga_make_entry(c, color);
}

static void scroll_if_needed(vga_virtual_context_t* ctx) {
    if (ctx->cursor_y >= ctx->viewport_y + VGA_HW_HEIGHT) {
        ctx->viewport_y = ctx->cursor_y - VGA_HW_HEIGHT + 1;
        if (ctx->viewport_y > VGA_VIRTUAL_HEIGHT - VGA_HW_HEIGHT)
            ctx->viewport_y = VGA_VIRTUAL_HEIGHT - VGA_HW_HEIGHT;
        vga_virtual_flush(ctx);
    }
}

static void put_char_at_cursor(vga_virtual_context_t* ctx, char c, uint8_t color) {
    if (c == '\n') {
        ctx->cursor_x = 0;
        ctx->cursor_y++;
    } else {
        vga_virtual_put_char(ctx, ctx->cursor_x, ctx->cursor_y, c, color);
        ctx->cursor_x++;
        if (ctx->cursor_x >= VGA_VIRTUAL_WIDTH) {
            ctx->cursor_x = 0;
            ctx->cursor_y++;
        }
    }
    scroll_if_needed(ctx);
}

void vga_virtual_write(vga_virtual_context_t* ctx, const char* str, uint8_t color) {
    for (size_t i = 0; str[i]; i++) {
        put_char_at_cursor(ctx, str[i], color);
    }
    vga_virtual_flush(ctx);
}

void vga_virtual_scroll_viewport(vga_virtual_context_t* ctx, int lines) {
    ctx->viewport_y += lines;
    if (ctx->viewport_y < 0) ctx->viewport_y = 0;
    if (ctx->viewport_y > VGA_VIRTUAL_HEIGHT - VGA_HW_HEIGHT)
        ctx->viewport_y = VGA_VIRTUAL_HEIGHT - VGA_HW_HEIGHT;

    vga_virtual_flush(ctx);
}

void vga_virtual_flush(vga_virtual_context_t* ctx) {
    for (int y = 0; y < VGA_HW_HEIGHT; y++) {
        for (int x = 0; x < VGA_HW_WIDTH; x++) {
            int virtual_x = ctx->viewport_x + x;
            int virtual_y = ctx->viewport_y + y;

            if (virtual_x >= VGA_VIRTUAL_WIDTH || virtual_y >= VGA_VIRTUAL_HEIGHT) {
                ctx->vga_mem[y * VGA_HW_WIDTH + x] = vga_make_entry(' ', 0x07);
            } else {
                ctx->vga_mem[y * VGA_HW_WIDTH + x] = ctx->backbuffer[virtual_y * VGA_VIRTUAL_WIDTH + virtual_x];
            }
        }
    }
}
