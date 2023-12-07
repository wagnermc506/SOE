#include "models.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>

PROVA_DATA* model_create_prova_data() {
    PROVA_DATA* data = malloc(sizeof(PROVA_DATA));
    memset(data, 0, sizeof(PROVA_DATA));
    return data;
}

void model_fill_prova_data_from_json(PROVA_DATA* data, cJSON* json) {
    cJSON* json_data_id = cJSON_GetObjectItem(json, "id");
    cJSON* json_data_local = cJSON_GetObjectItem(json, "local_aplicacao");
    cJSON* json_data_inicio = cJSON_GetObjectItem(json, "horario_inicio");
    cJSON* json_data_fim = cJSON_GetObjectItem(json, "horario_fim");

    strcpy(data->id, cJSON_GetStringValue(json_data_id));
    strcpy(data->local, cJSON_GetStringValue(json_data_local));
    strcpy(data->horario_inicio, cJSON_GetStringValue(json_data_inicio));
    strcpy(data->horario_fim, cJSON_GetStringValue(json_data_fim));
}

void model_fill_prova_data_list_from_json(PROVA_DATA* provas, int list_size, cJSON* json) {
    for (int i = 0; i < list_size; i++) {
        model_fill_prova_data_from_json(&provas[i], cJSON_GetArrayItem(json, i));
    }
}

void model_free_prova_data(PROVA_DATA* data) {
    free(data);
}
