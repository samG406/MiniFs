#ifndef DISK_H
#define DISK_H

#define BLOCK_SIZE 1024
#define NUM_BLOCKS 1024
#define DISK_FILE "minifs.disk"

int init_disk(const char *filename);
int read_block(int block_num, void *buffer);
int write_block(int block_num, const void *buffer);
void shutdown_disk();

#endif
