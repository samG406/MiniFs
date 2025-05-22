#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"

static FILE *disk_fp = NULL;

int init_disk(const char *filename) {
    disk_fp = fopen(filename, "r+b");
    if (!disk_fp) {
        disk_fp = fopen(filename, "w+b");
        if (!disk_fp) {
            perror("Failed to create disk file");
            return -1;
        }

        char zero_block[BLOCK_SIZE] = {0};
        for (int i = 0; i < NUM_BLOCKS; i++) {
            fwrite(zero_block, BLOCK_SIZE, 1, disk_fp);
        }
        fflush(disk_fp);
    }
    return 0;
}

int read_block(int block_num, void *buffer) {
    if (!disk_fp || block_num >= NUM_BLOCKS) return -1;
    fseek(disk_fp, block_num * BLOCK_SIZE, SEEK_SET);
    return fread(buffer, BLOCK_SIZE, 1, disk_fp) == 1 ? 0 : -1;
}

int write_block(int block_num, const void *buffer) {
    if (!disk_fp || block_num >= NUM_BLOCKS) return -1;
    fseek(disk_fp, block_num * BLOCK_SIZE, SEEK_SET);
    return fwrite(buffer, BLOCK_SIZE, 1, disk_fp) == 1 ? 0 : -1;
}

void shutdown_disk() {
    if (disk_fp) {
        fclose(disk_fp);
        disk_fp = NULL;
    }
}
