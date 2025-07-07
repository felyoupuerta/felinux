#include <stdint.h>
#include "keyboard.h"
#include "screen.h"
#include "commands.h"

#define KEYBOARD_DATA_PORT 0x60
#define ENTER_KEY 0x1C
#define BACKSPACE_KEY 0x0E
#define BUFFER_SIZE 128

char buffer[BUFFER_SIZE];
unsigned int index = 0;

// Array para almacenar manejadores de interrupciones
static void (*interrupt_handlers[256])(void);

// Función simple para convertir a hexadecimal (reemplaza sprintf)
static void uint8_to_hex(uint8_t value, char* buffer) {
    const char hex_chars[] = "0123456789ABCDEF";
    buffer[0] = '[';
    buffer[1] = hex_chars[(value >> 4) & 0xF];
    buffer[2] = hex_chars[value & 0xF];
    buffer[3] = ']';
    buffer[4] = '\0';
}

// Mapeo de scancodes a caracteres (layout QWERTY)
static const char scancode_to_char[] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0', [0x10] = 'q', [0x11] = 'w',
    [0x12] = 'e', [0x13] = 'r', [0x14] = 't', [0x15] = 'y',
    [0x16] = 'u', [0x17] = 'i', [0x18] = 'o', [0x19] = 'p',
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f',
    [0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k',
    [0x26] = 'l', [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c',
    [0x2F] = 'v', [0x30] = 'b', [0x31] = 'n', [0x32] = 'm',
    [0x39] = ' '
};

// Función para leer del puerto
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Función para escribir en un puerto (necesario para EOI si usas PIC)
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Manejador de interrupción de teclado (IRQ1)
void keyboard_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Ignorar teclas liberadas (bit 7 en 1)
    if (scancode & 0x80) {
        return;
    }

    if (scancode == ENTER_KEY) {
        buffer[index] = '\0';
        print("\n");
        handle_command(buffer);
        index = 0;
    } else if (scancode == BACKSPACE_KEY && index > 0) {
        index--;
        print("\b \b");
    } else if (scancode <= 0x39 && scancode_to_char[scancode] != 0) {
        if (index < BUFFER_SIZE - 1) {
            char c = scancode_to_char[scancode];
            buffer[index++] = c;
            char str[2] = {c, '\0'};
            print(str);
        }
    } else {
        char msg[5];
        uint8_to_hex(scancode, msg);
        print(msg);
    }

    // Enviar End of Interrupt
    outb(0x20, 0x20);
}

// Función para registrar manejadores de interrupciones
void register_interrupt_handler(uint8_t interrupt_num, void (*handler)(void)) {
    if (interrupt_num < 256) {
        interrupt_handlers[interrupt_num] = handler;
    }
}

// Función de inicialización del teclado
void init_keyboard(void) {
    // Inicializar buffer
    index = 0;
    buffer[0] = '\0';
    
    // Inicializar array de manejadores
    for (int i = 0; i < 256; i++) {
        interrupt_handlers[i] = 0;
    }
    
    register_interrupt_handler(33, keyboard_handler); // 33 = 0x21 = IRQ1
}
