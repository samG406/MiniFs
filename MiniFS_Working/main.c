#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "disk.h"
#include "fs_core.h"
#include "cache.h"

void print_usage() {
    printf("Usage:\n");
    printf("  ./minifs init\n");
    printf("  ./minifs open <filename>\n");
    printf("  ./minifs write <filename> <data>\n");
    printf("  ./minifs read <filename>\n");
    printf("  ./minifs flush\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    if (init_disk(DISK_FILE) != 0) {
        printf("Failed to initialize disk.\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        FileEntry empty_table[MAX_FILES];
        memset(empty_table, 0, sizeof(empty_table));

        char *ptr = (char*)empty_table;
        for (int i = 0; i < 5; i++) {
            write_block(i, ptr + (i * BLOCK_SIZE));
        }

        printf("File system initialized.\n");

    } else if (strcmp(argv[1], "open") == 0) {
        if (argc < 3) {
            printf("Missing filename.\n");
        } else {
            if (init_fs() != 0) {
                printf("Failed to load file system.\n");
                shutdown_disk();
                return 1;
            }

            int index = find_file(argv[2]);
            if (index == -1) {
                index = create_file(argv[2]);
                if (index != -1)
                    printf("File '%s' created.\n", argv[2]);
            } else {
                printf("File '%s' already exists.\n", argv[2]);
            }
        }

    } else if (strcmp(argv[1], "write") == 0) {
        if (argc < 4) {
            printf("Usage: ./minifs write <filename> <data>\n");
        } else {
            if (init_fs() != 0) {
                printf("Failed to load file system.\n");
                return 1;
            }

            cache_init();

            int index = find_file(argv[2]);
            if (index == -1) {
                printf("File not found.\n");
            } else {
                FileEntry* file = get_file_entry(index);

                int block = 10 + index;  // Simple block mapping
                file->block_pointers[0] = block;
                file->size = strlen(argv[3]);

                char block_data[BLOCK_SIZE] = {0};
                strncpy(block_data, argv[3], BLOCK_SIZE - 1);

                printf("[DEBUG] Writing to block: %d\n", block);
                cache_write(block, block_data);
                cache_flush();  // ✅ ensure persistence
                save_file_table();

                printf("Data written to '%s'.\n", argv[2]);
            }
        }

    } else if (strcmp(argv[1], "read") == 0) {
        if (argc < 3) {
            printf("Usage: ./minifs read <filename>\n");
        } else {
            if (init_fs() != 0) {
                printf("Failed to load file system.\n");
                return 1;
            }

            cache_init();

            int index = find_file(argv[2]);
            if (index == -1) {
                printf("File not found.\n");
            } else {
                FileEntry* file = get_file_entry(index);
                int block = file->block_pointers[0];
                printf("[DEBUG] Reading from block: %d\n", block);

                if (block < 0 || block >= NUM_BLOCKS) {
                    printf("Invalid block pointer.\n");
                } else {
                    char buffer[BLOCK_SIZE] = {0};
                    if (cache_read(block, buffer) == 0) {
                        printf("Data: %s\n", buffer);
                    } else {
                        printf("Reading from disk...\n");
                        if (read_block(block, buffer) == 0)
                            printf("Data: %s\n", buffer);
                        else
                            printf("Failed to read block.\n");
                    }
                }
            }
        }

    } else if (strcmp(argv[1], "flush") == 0) {
        cache_flush();
        printf("Cache flushed to disk.\n");

    } else {
        print_usage();
    }

    shutdown_disk();
    return 0;
}
