#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_TOKENS 100

void parse_input(char *input, char **tokens, char *delimiter);

int main() {
    char input[MAX_INPUT_LENGTH];
    char *tokens[MAX_TOKENS];
    pid_t pid;
    int status;

    while (1) {
        printf("MyShell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (input[0] == '\0') {
            continue;
        }

        pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            parse_input(input, tokens, " ");
            if (execvp(tokens[0], tokens) == -1) {
                perror("Execvp failed");
                exit(1);
            }
        } else {
            wait(&status);
        }
    }

    return 0;
}

void parse_input(char *input, char **tokens, char *delimiter) {
    char *token = strtok(input, delimiter);
    int i = 0;
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, delimiter);
    }
    tokens[i] = NULL;
}
