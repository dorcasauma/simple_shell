#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMANDS 10
#define MAX_COMMAND_LENGTH 100

int main() {
    char input[MAX_COMMAND_LENGTH];
    char *commands[MAX_COMMANDS];
    int num_commands = 0;

    char *token;
    size_t len;
    int i;

    printf("Enter commands (separated by ;): ");
    fgets(input, sizeof(input), stdin);

    token = strtok(input, ";");
    while (token != NULL && num_commands < MAX_COMMANDS) {
        commands[num_commands] = strdup(token);
        num_commands++;
        token = strtok(NULL, ";");
    }

    for (i = 0; i < num_commands; i++) {
        char *command = commands[i];
        int status;

        while (*command == ' ' || *command == '\t') {
            command++;
        }

        len = strlen(command);

        while (len > 0 && (command[len - 1] == ' ' || command[len - 1] == '\t' || command[len - 1] == '\n')) {
            command[len - 1] = '\0';
            len--;
        }

        if (strlen(command) > 0) {
            pid_t pid = fork();
            if (pid == 0) {
                execlp("/bin/sh", "/bin/sh", "-c", command, NULL);
                perror("exec");
                exit(1);
            } else if (pid < 0) {
                perror("fork");
                exit(1);
            } else {
                waitpid(pid, &status, 0);
            }
        }

        free(commands[i]);
    }

    return 0;
}
