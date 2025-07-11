#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

void clear_screen();
void print(const char* text);
void clear_screen();
void print(const char* str);
void print_char(char c);
void set_cursor_pos(uint8_t x, uint8_t y);  // Cambiado de int a uint8_t
uint16_t get_cursor_pos();                  // Devuelve posición empaquetada
void set_text_color(uint8_t fg, uint8_t bg);
void print_backspace();
#endif
