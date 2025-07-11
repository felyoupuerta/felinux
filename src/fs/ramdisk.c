#include "ramdisk.h"
#include <string.h>

static uint8_t ramdisk[RAMDISK_SIZE];

void ramdisk_init() {
    memset(ramdisk, 0, sizeof(ramdisk));
}

void read_block(uint32_t block_num, void *buffer) {
    if (block_num >= TOTAL_BLOCKS) return;
    memcpy(buffer, &ramdisk[block_num * BLOCK_SIZE], BLOCK_SIZE);
}

void write_block(uint32_t block_num, const void *data) {
    if (block_num >= TOTAL_BLOCKS) return;
    memcpy(&ramdisk[block_num * BLOCK_SIZE], data, BLOCK_SIZE);
}
