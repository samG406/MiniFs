#ifndef CACHE_H
#define CACHE_H

#define CACHE_SIZE 32

void cache_init();
int cache_read(int block_num, void *buffer);
void cache_write(int block_num, const void *data);
void cache_flush();

#endif
