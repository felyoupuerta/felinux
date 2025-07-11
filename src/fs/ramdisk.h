#ifndef RAMDISK_H
#define RAMDISK_H

#include <stdint.h>

#define RAMDISK_SIZE (1024 * 1024) // 1 MB
#define BLOCK_SIZE 512
#define TOTAL_BLOCKS (RAMDISK_SIZE / BLOCK_SIZE)

void ramdisk_init(void);
void read_block(uint32_t block_num, void *buffer);
void write_block(uint32_t block_num, const void *data);

#endif
