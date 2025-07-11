#include "system.h"
#include "ports.h"  


void cli(void) {
    __asm__ volatile ("cli");
}


void sti(void) {
    __asm__ volatile ("sti");
}


void halt(void) {
    __asm__ volatile ("hlt");
}


void reboot(void) {
    
    uint8_t temp = 0x02;
    while (temp & 0x02) {
        temp = inb(0x64);
    }
    outb(0x64, 0xFE);
    halt();  
}
