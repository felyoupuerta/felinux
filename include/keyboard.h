#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void init_keyboard(void);
void register_interrupt_handler(uint8_t interrupt_num, void (*handler)(void));

#endif
