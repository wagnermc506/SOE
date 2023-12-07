#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>

enum soe_data_type {
    PROVA = 0,
    USUARIO = 1,
    FOTO_USUARIO = 2,
    NONE = 3
};

// void save_data(char* path, void* buffer, size_t size_buffer);
void save_data(char* path, void* buffer, size_t size_buffer, enum soe_data_type dt);
void load_data(char* path, void* buffer, size_t size_buffer, enum soe_data_type dt);

#endif
