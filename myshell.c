#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 64

void cleanupAndExit(char *userInput, int exit_status);
void executeCommand(char **shellArgs);

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream);

int main(void) {
    char *userInput = NULL;
    size_t userInputSize = 0;
    int exit_status = 0;
    char *shellArgs[MAX_ARGS];

    ssize_t bytesRead;

    while (1) {
        printf("$ ");
        bytesRead = custom_getline(&userInput, &userInputSize, stdin);

        if (bytesRead == -1) {
            perror("getline");
            exit(1);
        }

        if (bytesRead == 0) {
            break;
        }


        if (shellArgs[0] != NULL) {
            if (strcmp(shellArgs[0], "exit") == 0) {
                if (shellArgs[1] != NULL) {
                    exit_status = atoi(shellArgs[1]);
                }
                break;
            } else if (strcmp(shellArgs[0], "setenv") == 0) {
                if (shellArgs[1] != NULL && shellArgs[2] != NULL) {
                    if (setenv(shellArgs[1], shellArgs[2], 1) != 0) {
                        fprintf(stderr, "setenv: Failed to set environment variable.\n");
                    }
                } else {
                    fprintf(stderr, "setenv: Usage: setenv VARIABLE VALUE\n");
                }
            } else if (strcmp(shellArgs[0], "unsetenv") == 0) {
                if (shellArgs[1] != NULL) {
                    if (unsetenv(shellArgs[1]) != 0) {
                        fprintf(stderr, "unsetenv: Failed to unset environment variable.\n");
                    }
                } else {
                    fprintf(stderr, "unsetenv: Usage: unsetenv VARIABLE\n");
                }
            } else {
                executeCommand(shellArgs);
            }
        }

        cleanupAndExit(userInput, exit_status);
    }

    cleanupAndExit(userInput, exit_status);
}

void cleanupAndExit(char *userInput, int exit_status) {
    free(userInput);
    exit(exit_status);
}

void executeCommand(char **shellArgs) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        if (execvp(shellArgs[0], shellArgs) == -1) {
            perror("execvp");
            exit(1);
        }
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
        }
    }
}

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream) {
    ssize_t bytesRead;
    *lineptr = NULL;
    *n = 0;

    bytesRead = getline(lineptr, n, stream);
    return bytesRead;
}
