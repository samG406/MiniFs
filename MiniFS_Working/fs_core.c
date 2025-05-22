#include <stdio.h>
#include <string.h>
#include "fs_core.h"
#include "disk.h"

static FileEntry file_table[MAX_FILES];

int init_fs() {
    char *ptr = (char*)file_table;
    for (int i = 0; i < 5; i++) {
        char block[BLOCK_SIZE];
        if (read_block(i, block) != 0) return -1;
        memcpy(ptr + (i * BLOCK_SIZE), block, BLOCK_SIZE);
    }
    return 0;
}

void save_file_table() {
    char *ptr = (char*)file_table;
    for (int i = 0; i < 5; i++) {
        write_block(i, ptr + (i * BLOCK_SIZE));
    }
}

int find_file(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].used && strcmp(file_table[i].name, filename) == 0) {
            return i;
        }
    }
    return -1;
}

int create_file(const char *filename) {
    if (find_file(filename) != -1) {
        printf("File already exists.\n");
        return -1;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!file_table[i].used) {
            strncpy(file_table[i].name, filename, sizeof(file_table[i].name) - 1);
            file_table[i].name[sizeof(file_table[i].name) - 1] = '\0';
            file_table[i].size = 0;

            for (int j = 0; j < MAX_BLOCKS_PER_FILE; j++) {
                file_table[i].block_pointers[j] = -1;
            }

            file_table[i].used = 1;
            save_file_table();
            return i;
        }
    }

    printf("File table full.\n");
    return -1;
}

FileEntry* get_file_entry(int index) {
    if (index < 0 || index >= MAX_FILES || !file_table[index].used) return NULL;
    return &file_table[index];
}
