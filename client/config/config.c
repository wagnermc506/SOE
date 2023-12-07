#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "config.h"

int verify_envs() {
    char* internal_database_path_dir = getenv("INTERNAL_DATABASE_PATH_DIR");
    if (internal_database_path_dir == NULL) return 1;

    char* user_path_dir = malloc(strlen(internal_database_path_dir) + 10);
    memset(user_path_dir, 0, strlen(internal_database_path_dir) + 10);
    strcat(user_path_dir, internal_database_path_dir);
    strcat(user_path_dir, "usuarios");

    char* user_photo_path_dir = malloc(strlen(internal_database_path_dir) + 10);
    memset(user_photo_path_dir, 0, strlen(internal_database_path_dir) + 10);
    strcat(user_photo_path_dir, internal_database_path_dir);
    strcat(user_photo_path_dir, "fotos");

    struct stat st;

    int create_database_path_dir = 0;
    int create_user_path_dir = 0;
    int create_user_photo_path_dir = 0;

    if (stat(internal_database_path_dir, &st) != 0) {
        create_database_path_dir = mkdir(internal_database_path_dir, 0777);
    }
    else create_database_path_dir = !S_ISDIR(st.st_mode);

    if (stat(user_path_dir, &st) != 0) {
        create_user_path_dir = mkdir(user_path_dir, 0777);
        free(user_path_dir);
    }
    else create_user_path_dir = !S_ISDIR(st.st_mode);

    if (stat(user_photo_path_dir, &st) != 0) {
        create_user_photo_path_dir = mkdir(user_photo_path_dir, 0777);
        free(user_photo_path_dir);
    }
    else create_user_photo_path_dir = !S_ISDIR(st.st_mode);

    return create_database_path_dir || create_user_path_dir || create_user_photo_path_dir;
}

char* get_internal_database_path_dir() {
    return getenv("INTERNAL_DATABASE_PATH_DIR");
}
