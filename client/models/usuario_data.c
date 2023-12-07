#include "models.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>

USUARIO_DATA* model_create_usuario_data() {
    USUARIO_DATA* data = malloc(sizeof(USUARIO_DATA));
    memset(data, 0, sizeof(USUARIO_DATA));
    return data;
}

void model_fill_usuario_data_from_json(USUARIO_DATA* data, cJSON* json) {
    cJSON* json_data_nome = cJSON_GetObjectItem(json, "nome");
    cJSON* json_data_matricula = cJSON_GetObjectItem(json, "matricula");
    cJSON* json_data_cpf = cJSON_GetObjectItem(json, "cpf");

    strcpy(data->nome, cJSON_GetStringValue(json_data_nome));
    data->matricula = cJSON_GetNumberValue(json_data_matricula);
    strcpy(data->cpf, cJSON_GetStringValue(json_data_cpf));
}

void model_fill_usuario_data_list_from_json(USUARIO_DATA* usuarios, int list_size, cJSON* json) {
    for (int i = 0; i < list_size; i++) {
        model_fill_usuario_data_from_json(&usuarios[i], cJSON_GetArrayItem(json, i));
    }
}

void model_free_usuario_data(USUARIO_DATA* data) {
    free(data);
}
