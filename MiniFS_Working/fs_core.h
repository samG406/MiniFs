#ifndef FS_CORE_H
#define FS_CORE_H

#define MAX_FILES 64
#define MAX_BLOCKS_PER_FILE 8

typedef struct {
    char name[32];
    int size;
    int block_pointers[MAX_BLOCKS_PER_FILE];
    int used; // 0 = free, 1 = in use
} FileEntry;

int init_fs();
int create_file(const char *filename);
int find_file(const char *filename);
FileEntry* get_file_entry(int index);
void save_file_table();

#endif
