#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../api_consumer/api_consumer.h"
#include "../database/database.h"
#include "../models/models.h"

void list_provas() {
    int n = 0;
    PROVA_DATA* provas = soe_get_provas(&n);

    for (int i = 0; i < n; i++) {
        printf("%s >> %s [%s - %s]\n", provas[i].id, provas[i].local, provas[i].horario_inicio, provas[i].horario_fim);
    }
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

    char* prova_id = argv[1];

    PROVA_DATA* prova = soe_get_prova(prova_id);

    int size_usuarios = 0;
    USUARIO_DATA* usuarios = soe_get_usuarios_from_prova(prova_id, &size_usuarios);

    // pega as imagens dos usuários
    for (int i = 0; i < size_usuarios; i++) {
        size_t size_image;
        void* image = soe_get_usuario_foto(usuarios[i].cpf, &size_image);

        char filename[50];
        memset(filename, 0, 50);
        strcat(filename, usuarios[i].cpf);
        strcat(filename, ".jpg");

        save_data(filename, image, size_image);

        if (image != NULL) {
            free(image);
        }
    }

    save_data("./prova.db", (void*)prova, sizeof(PROVA_DATA));
    save_data("./usuarios.db", (void*)usuarios, sizeof(USUARIO_DATA)*size_usuarios);

    free(prova);
    free(usuarios);

    return 0;
}
