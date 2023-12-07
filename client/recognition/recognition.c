#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <regex.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>
#include <cjson/cJSON.h>

#include "recognition.h"
#include "../database/database.h"
#include "../models/models.h"

static int pipefd[2];
static pid_t child_pid = 0;
static int stop = 0;

void save_recognized_user(char* buffer) {
	cJSON* user_json = cJSON_Parse(buffer);
	if (user_json == NULL) return;

	cJSON* user_file_path_json = cJSON_GetObjectItem(user_json, "recognition");
	char* user_file_path = cJSON_GetStringValue(user_file_path_json);

	char* base_path = basename(user_file_path);
	char* base_path_dot = strrchr(base_path, '.');

	if (base_path_dot != NULL) {
		*base_path_dot = '\0';
	}

	char user_path[128];
	memset(user_path, 0, 128);
	strcat(user_path, base_path);
	strcat(user_path, ".db");

	USUARIO_DATA* user_data = model_create_usuario_data();
	load_data(user_path, user_data, sizeof(USUARIO_DATA), USUARIO);

	user_data->autenticado = 1;
	save_data(user_path, user_data, sizeof(USUARIO_DATA), USUARIO);

	model_free_usuario_data(user_data);
	cJSON_Delete(user_json);
}

void handle_sigint(int sig) {
    printf("Killing child process and exiting recognition...\n");
    kill(child_pid, SIGINT);
    stop = 1;
}

void child_process() {
    while ((dup2(pipefd[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
    close(pipefd[1]);
    close(pipefd[0]);
    execl("/usr/bin/python", "/usr/bin/python", "/home/frostwagner/Documents/soe/image.py", (char*)NULL);
    perror("execl");
    _exit(1);
}

void filter_logs() {
    regex_t re;
	regmatch_t match;

    char buffer[4096];
	char msg[4096];

    int comp_result = regcomp(&re, "recognition: {.*}", 0);
	if (comp_result != 0) {
		perror("regcomp");
	}

    while (!stop) {
		ssize_t count = read(pipefd[0], buffer, sizeof(buffer));
		if (count == -1) {
			if (errno == -1) {
				continue;
			}
			else {
				perror("read");
				exit(1);
			}
		}
		else if (count == 0) {
			break;
		}
		else {
			int ismatch = regexec(&re, buffer, 1, &match, 0);
			if (ismatch == 0) {
				size_t n = match.rm_eo - match.rm_so + 1;
				memset(msg, 0, 4096);
				strncpy(msg, &buffer[match.rm_so], n);
				printf("%s", msg);
				save_recognized_user(msg);
			}
		}
	}
}

int run_recognition() {
    signal(SIGINT, handle_sigint);

    char* python_unbuffered_env = "PYTHONUNBUFFERED=true";
	putenv(python_unbuffered_env);

	if (pipe(pipefd) == -1) {
		perror("Nao foi possivel criar pipe");
		return 1;
	}

    child_pid = fork();

    if (child_pid == -1) {
		perror("Nao foi possivel criar fork");
		return 2;
	}

	if (child_pid == 0) child_process();

    close(pipefd[1]);

    filter_logs();

	close(pipefd[0]);
	wait(0);

    signal(SIGINT, SIG_DFL);

	return 0;
}
