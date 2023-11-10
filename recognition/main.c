#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <regex.h>
#include <string.h>

int main() {

	char* python_unbuffered_env = "PYTHONUNBUFFERED=true";
	putenv(python_unbuffered_env);

	int pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(1);
	}

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		exit(1);
	}

	if (pid == 0) {
		while ((dup2(pipefd[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
		close(pipefd[1]);
		close(pipefd[0]);
		execl("/usr/bin/python", "/usr/bin/python", "/home/frostwagner/Documents/soe/image.py", (char*)NULL);
		perror("execl");
		_exit(1);
	}

	close(pipefd[1]);

	regex_t re;
	regmatch_t match;

	int comp_result = regcomp(&re, "recognition: {.*}", 0);
	if (comp_result != 0) {
		perror("regcomp");
	}


	char buffer[4096];
	char msg[4096];
	while (1) {
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
			}
		}
	}
	close(pipefd[0]);
	wait(0);

}
