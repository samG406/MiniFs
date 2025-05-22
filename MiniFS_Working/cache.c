#include <string.h>
#include "cache.h"
#include "disk.h"

typedef struct {
    int block_num;
    char data[BLOCK_SIZE];
    int dirty;
    int valid;
} CacheBlock;

static CacheBlock cache[CACHE_SIZE];

void cache_init() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache[i].valid = 0;
        cache[i].dirty = 0;
        cache[i].block_num = -1;
    }
}

int cache_read(int block_num, void *buffer) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache[i].valid && cache[i].block_num == block_num) {
            memcpy(buffer, cache[i].data, BLOCK_SIZE);
            return 0;
        }
    }

    // Not found in cache, read from disk
    if (read_block(block_num, buffer) == 0) {
        return 0;
    }
    return -1;
}

void cache_write(int block_num, const void *data) {
    // Check if already in cache
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache[i].valid && cache[i].block_num == block_num) {
            memcpy(cache[i].data, data, BLOCK_SIZE);
            cache[i].dirty = 1;
            return;
        }
    }

    // Find an empty slot
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (!cache[i].valid) {
            cache[i].valid = 1;
            cache[i].block_num = block_num;
            cache[i].dirty = 1;
            memcpy(cache[i].data, data, BLOCK_SIZE);
            return;
        }
    }

    // Cache full: overwrite first entry
    cache[0].valid = 1;
    cache[0].block_num = block_num;
    cache[0].dirty = 1;
    memcpy(cache[0].data, data, BLOCK_SIZE);
}

void cache_flush() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache[i].valid && cache[i].dirty) {
            write_block(cache[i].block_num, cache[i].data);
            cache[i].dirty = 0;
        }
    }
}
