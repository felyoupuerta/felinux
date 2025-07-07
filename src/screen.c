#include <stdint.h>
#include "screen.h"

#define VIDEO_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F
uint16_t* video = (uint16_t*) VIDEO_ADDRESS;
int cursor = 0;

void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        video[i] = (WHITE_ON_BLACK << 8) | ' ';
    }
    cursor = 0;
}

void print(const char* str) {
    while (*str) {
        video[cursor++] = (WHITE_ON_BLACK << 8) | *str++;
    }
}
