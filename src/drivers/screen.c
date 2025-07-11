#include "screen.h"
#include "ports.h"
#include "../include/system.h"

static volatile uint16_t* video = (uint16_t*)0xB8000;
static uint8_t text_attr = 0x07;

/* Función interna para manejar offset */
static void set_cursor_offset(uint16_t offset) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(offset & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((offset >> 8) & 0xFF));
}

void set_cursor_pos(uint8_t x, uint8_t y) {
    if (x >= SCREEN_WIDTH) x = SCREEN_WIDTH - 1;
    if (y >= SCREEN_HEIGHT) y = SCREEN_HEIGHT - 1;
    set_cursor_offset(y * SCREEN_WIDTH + x);
}

uint16_t get_cursor_pos() {
    uint16_t offset = 0;
    outb(0x3D4, 0x0F);
    offset |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    offset |= inb(0x3D5) << 8;
    return (offset % SCREEN_WIDTH) | ((offset / SCREEN_WIDTH) << 8);
}

void clear_screen() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video[i] = (text_attr << 8) | ' ';
    }
    set_cursor_pos(0, 0);
}

void print(const char* str) {
    uint16_t pos = get_cursor_pos();
    uint8_t x = pos & 0xFF;
    uint8_t y = (pos >> 8) & 0xFF;
    
    while (*str) {
        if (*str == '\n') {
            x = 0;
            y++;
        } else {
            video[y * SCREEN_WIDTH + x] = (text_attr << 8) | *str;
            x++;
        }
        
        if (x >= SCREEN_WIDTH) {
            x = 0;
            y++;
        }
        if (y >= SCREEN_HEIGHT) {
            y = SCREEN_HEIGHT - 1;
            // Aquí deberías implementar scroll_screen() si es necesario
        }
        str++;
    }
    set_cursor_pos(x, y);
}

void print_char(char c) {
    uint16_t pos = get_cursor_pos();
    uint8_t x = pos & 0xFF;
    uint8_t y = (pos >> 8) & 0xFF;
    
    if (c == '\n') {
        x = 0;
        y++;
    } else {
        video[y * SCREEN_WIDTH + x] = (text_attr << 8) | c;
        x++;
    }
    
    // Manejo de límites
    if (x >= SCREEN_WIDTH) {
        x = 0;
        y++;
    }
    if (y >= SCREEN_HEIGHT) {
        y = SCREEN_HEIGHT - 1;
        // scroll_screen();
    }
    
    set_cursor_pos(x, y);
}

void set_text_color(uint8_t fg, uint8_t bg) {
    text_attr = (bg << 4) | (fg & 0x0F);
}

void scroll_screen() {
    // Mover todas las líneas hacia arriba
    for (int y = 0; y < SCREEN_HEIGHT - 1; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            video[y * SCREEN_WIDTH + x] = video[(y + 1) * SCREEN_WIDTH + x];
        }
    }

    // Limpiar la última línea
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        video[(SCREEN_HEIGHT - 1) * SCREEN_WIDTH + x] = (text_attr << 8) | ' ';
    }

    set_cursor_pos(0, SCREEN_HEIGHT - 1);
}

void print_backspace() {
    uint16_t pos = get_cursor_pos();
    uint8_t x = pos & 0xFF;
    uint8_t y = (pos >> 8) & 0xFF;

    if (x == 0 && y == 0) return; // No hacer nada si ya estamos en (0,0)

    if (x > 0) {
        x--;
    } else if (y > 0) {
        y--;
        x = SCREEN_WIDTH - 1;
    }

    video[y * SCREEN_WIDTH + x] = (text_attr << 8) | ' ';
    set_cursor_pos(x, y);
}
