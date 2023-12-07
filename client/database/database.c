#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "database.h"
#include "../config/config.h"

char* handle_path(char* file, enum soe_data_type dt) {
    char* full_path = malloc(sizeof(char)*256);
    memset(full_path, 0, sizeof(char)*256);

    strcat(full_path, get_internal_database_path_dir());

    switch (dt) {
        case(PROVA):
            strcat(full_path, file);
            break;
        case(USUARIO):
            strcat(full_path, "usuarios/");
            strcat(full_path, file);
            break;
        case(FOTO_USUARIO):
            strcat(full_path, "fotos/");
            strcat(full_path, file);
            break;
        case(NONE):
            free(full_path);
            return file;
    }

    return full_path;
}

void free_path(char* path, enum soe_data_type dt) {
    switch (dt) {
        case(NONE):
            break;
        default:
            free(path);
            break;
    }
}

void save_data(char* path, void* buffer, size_t size_buffer, enum soe_data_type dt) {
    char* full_path = handle_path(path, dt);

    FILE* fd = fopen(full_path, "w+b");
    fwrite(buffer, size_buffer, 1, fd);
    fclose(fd);

    free_path(full_path, dt);
}

void load_data(char* path, void* buffer, size_t size_buffer, enum soe_data_type dt) {
    char* full_path = handle_path(path, dt);

    FILE* fd = fopen(full_path, "rb");
    fread(buffer, size_buffer, 1, fd);
    fclose(fd);

    free_path(full_path, dt);
}
