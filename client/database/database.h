#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>

void save_data(char* path, void* buffer, size_t size_buffer);
void load_data(char* path, void* buffer, size_t size_buffer);

#endif
