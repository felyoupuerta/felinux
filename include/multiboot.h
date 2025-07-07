#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER

#include <stdint.h>

#define MULTIBOOT_MAGIC 0x1BADB002
#define MULTIBOOT_FLAGS 0x00000003

__attribute__((section(".multiboot")))
volatile uint32_t multiboot_header[] = {
    0x1BADB002,          // Magic
    0x00000003,          // Flags (aliniación + info de memoria)
    0xE4524FFB           // Checksum (magic + flags)
};

#endif
