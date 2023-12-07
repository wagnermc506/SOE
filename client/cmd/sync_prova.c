#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../api_consumer/api_consumer.h"
#include "../database/database.h"
#include "../models/models.h"
#include "../config/config.h"

void list_provas() {
    int n = 0;
    PROVA_DATA* provas = soe_get_provas(&n);

    for (int i = 0; i < n; i++) {
        printf("%s >> %s [%s - %s]\n", provas[i].id, provas[i].local, provas[i].horario_inicio, provas[i].horario_fim);
    }
}

void save_user_image(USUARIO_DATA* usuario) {
    size_t size_image;
    void* image = soe_get_usuario_foto(usuario->cpf, &size_image);

    char filename[50];
    memset(filename, 0, 50);
    strcat(filename, usuario->cpf);
    strcat(filename, ".jpg");

    save_data(filename, image, size_image, FOTO_USUARIO);

    if (image != NULL) {
        free(image);
    }
}

void save_user_data(USUARIO_DATA* usuario) {
    char* filename = malloc(sizeof(char)*20);
    memset(filename, 0, sizeof(char)*20);
    strcat(filename, usuario->cpf);
    strcat(filename, ".db");

    save_data(filename, usuario, sizeof(USUARIO_DATA), USUARIO);

    free(filename);
}

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Usage: %s <id_prova>\n", argv[0]);
        return 0;
    }

    if (!strcmp(argv[1], "list")) {
        list_provas();
        return 0;
    }

    if (verify_envs() != 0) {
        printf("Env variable 'INTERNAL_DATABASE_PATH_DIR' not found\n");
        return 2;
    }

    char* prova_id = argv[1];

    PROVA_DATA* prova = soe_get_prova(prova_id);

    int size_usuarios = 0;
    USUARIO_DATA* usuarios = soe_get_usuarios_from_prova(prova_id, &size_usuarios);

    // salva dados da prova
    save_data("prova.db", (void*)prova, sizeof(PROVA_DATA), PROVA);

    // salva dados do usuário
    for (int i = 0; i < size_usuarios; i++) {
        save_user_data(&usuarios[i]);
        save_user_image(&usuarios[i]);
    }

    free(prova);
    free(usuarios);

    return 0;
}
