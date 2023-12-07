#define _XOPEN_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "../models/models.h"
#include "../database/database.h"
#include "../recognition/recognition.h"

#define TRUE  1
#define FALSE 0
#define SLEEP_TIME_SEC 5
#define PROVA_FILE "prova.db"
#define TIMESTAMP_FORMAT "%Y-%m-%dT%H:%M:%S"

static pid_t recognition_pid = 0;
static int8_t recognition_running = 0;

int start_running_recognition() {
    recognition_pid = fork();

    if (recognition_pid == -1) {
        return -1;
    }

    // le fork
    if (recognition_pid == 0) {
        run_recognition();
    }
    else {
        recognition_running = 1;
    }

    return 0;
}

void stop_running_recognition() {
    int exit_status;
    kill(recognition_pid, SIGINT);
    waitpid(recognition_pid, &exit_status, 0);
    recognition_pid = 0;
    recognition_running = 0;
}

int main() {

    PROVA_DATA* prova = model_create_prova_data();
    time_t current_time, prova_inicio, prova_fim;
    struct tm prova_time_inicio;
    struct tm prova_time_fim;

    while(1) {
        load_data(PROVA_FILE, prova, sizeof(PROVA_DATA), PROVA);

        strptime(prova->horario_inicio, TIMESTAMP_FORMAT, &prova_time_inicio);
        strptime(prova->horario_fim, TIMESTAMP_FORMAT, &prova_time_fim);

        time(&current_time);
        prova_inicio = mktime(&prova_time_inicio);
        prova_fim = mktime(&prova_time_fim);

        if (current_time >= prova_inicio && current_time <= prova_fim) {
            if (!recognition_running) start_running_recognition();
        }
        else {
            if (recognition_running) stop_running_recognition();
        }

        sleep(SLEEP_TIME_SEC);
    }

    model_free_prova_data(prova);
    return 0;
}
