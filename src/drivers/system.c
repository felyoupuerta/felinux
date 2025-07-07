#include "system.h"
#include "ports.h"  // Para posibles usos futuros

// Deshabilita interrupciones
void cli(void) {
    __asm__ volatile ("cli");
}

// Habilita interrupciones
void sti(void) {
    __asm__ volatile ("sti");
}

// Detiene la CPU indefinidamente
void halt(void) {
    __asm__ volatile ("hlt");
}

// Función opcional de reinicio (usando teclado controller)
void reboot(void) {
    // Envía señal de reset al controlador de teclado
    uint8_t temp = 0x02;
    while (temp & 0x02) {
        temp = inb(0x64);
    }
    outb(0x64, 0xFE);
    halt();  // Fallback si el reset no funciona
}
