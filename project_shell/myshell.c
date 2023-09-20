#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char *userInput = NULL;
    size_t userInputSize = 0;
    char *shellCommand = "/bin/bash";
    char *envp[] = {NULL};
    int execveResult;
    int status;
    pid_t my_pid;
    ssize_t bytesRead;

    while (1) {
        printf("mysimpleshell $: ");
        ssize_t bytesRead = getline(&userInput, &userInputSize, stdin);

        if (bytesRead == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }

        userInput[bytesRead - 1] = '\0';

        if (strcmp(userInput, "exit") == 0) {
            free(userInput);
            exit(0);
        }

        char *shellArgs[] = {shellCommand, "-c", userInput, NULL};

        my_pid = fork();

        if (my_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (my_pid == 0) {
            execveResult = execve(shellCommand, shellArgs, envp);
            perror("execve");
            free(userInput);  // Free allocated memory before exiting child process
            exit(EXIT_FAILURE);
        } else {
            waitpid(my_pid, &status, 0);
            printf("You entered: %s\n", userInput);
        }
    }

    free(userInput);  // Free allocated memory before exiting the main process
    return 0;
}
