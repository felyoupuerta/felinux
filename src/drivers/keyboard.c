#include <keyboard.h>    // Uso correcto de <> para headers del sistema
#include <screen.h>      // Uso correcto de <>
#include <system.h>      // Cambiado de "../include/system.h" a <system.h>
#include <ports.h>       // Añadido para inb()

static char keyboard_map[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0,' ',0,
};

// Verifica si hay datos disponibles en el teclado
static int keyboard_has_data() {
    return inb(0x64) & 0x01;
}

unsigned char read_scan_code() {
    if (!keyboard_has_data()) return 0; // No hay datos nuevos
    
    unsigned char scan = inb(0x60);
    
    // Limpia cualquier dato pendiente en el buffer
    while (keyboard_has_data()) inb(0x60);
    
    if (scan & 0x80) return 0; // Ignorar key up
    if (scan >= 128) return 0; // Scancode inválido
    
    return keyboard_map[scan];
}

void read_input(char* buffer, int max) {
    int i = 0;
    while (i < max - 1) {
        unsigned char c = read_scan_code();
        if (c) {
            if (c == '\n') {
                print("\n");
                break;
            }
            if (c == '\b' && i > 0) {
                i--;
                print("\b \b");
            } else if (c != '\b') {
                buffer[i++] = c;
                char str[2] = {c, 0};
                print(str);
            }
        }
    }
    buffer[i] = '\0';
}
