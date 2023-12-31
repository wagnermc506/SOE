#ifndef _MODELS_H_
#define _MODELS_H_

#include <cjson/cJSON.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct PROVA_DATA {
    char id[37];
    char local[200];
    char horario_inicio[20];
    char horario_fim[20];
    int status;
} PROVA_DATA;

PROVA_DATA* model_create_prova_data();
void model_fill_prova_data_from_json(PROVA_DATA*, cJSON*);
void model_fill_prova_data_list_from_json(PROVA_DATA* provas, int list_size, cJSON* json);
void model_free_prova_data(PROVA_DATA*);

typedef struct USUARIO_DATA {
    char nome[150];
    int matricula;
    char cpf[12];
    char autenticado;
    char autenticated_time[20];
} USUARIO_DATA;

USUARIO_DATA* model_create_usuario_data();
void model_fill_usuario_data_from_json(USUARIO_DATA*, cJSON*);
void model_fill_usuario_data_list_from_json(USUARIO_DATA* usuarios, int list_size, cJSON* json);
void model_free_usuario_data(USUARIO_DATA*);

static inline int get_list_size(cJSON* json) {
    return cJSON_GetArraySize(json);
}

#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"
static inline void get_timestamp_now(char* buffer) {
    time_t current_time;
    time(&current_time);

    struct tm current;
    localtime(&current_time);

    strftime(buffer, 20, TIME_FORMAT, &current);
}
#undef TIME_FORMAT

#endif
