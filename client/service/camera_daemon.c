#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../models/models.h"
#include "../database/database.h"

#define TRUE  1
#define FALSE 0
#define SLEEP_TIME_SEC 5
#define PROVA_FILE "prova.db"
#define TIMESTAMP_FORMAT "%Y-%m-%dT%H:%M:%S"

int main() {

    PROVA_DATA* prova = model_create_prova_data();
    time_t current_time, prova_inicio, prova_fim;
    struct tm prova_time_inicio;
    struct tm prova_time_fim;

    while(1) {
        load_data(PROVA_FILE, prova, sizeof(PROVA_DATA));

        strptime(prova->horario_inicio, TIMESTAMP_FORMAT, &prova_time_inicio);
        strptime(prova->horario_fim, TIMESTAMP_FORMAT, &prova_time_fim);

        time(&current_time);
        prova_inicio = mktime(&prova_time_inicio);
        prova_fim = mktime(&prova_time_fim);

        if (current_time >= prova_inicio && current_time <= prova_fim) {
            printf("PROVA TIME\n");
        }
        else if (current_time > prova_fim) {
            printf("CABOU\n");
        }

        sleep(SLEEP_TIME_SEC);
    }

    model_free_prova_data(prova);
    return 0;
}
