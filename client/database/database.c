#include "database.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void save_data(char* path, void* buffer, size_t size_buffer) {
    FILE* fd = fopen(path, "w+b");
    fwrite(buffer, size_buffer, 1, fd);
    fclose(fd);
}

void load_data(char* path, void* buffer, size_t size_buffer) {
    FILE* fd = fopen(path, "rb");
    fread(buffer, size_buffer, 1, fd);
    fclose(fd);
}
