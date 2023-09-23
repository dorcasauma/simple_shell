#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

void executeCommand(char *userInput) {
    char *args[MAX_BUFFER_SIZE];
    int i = 0;
    char currentDir[MAX_BUFFER_SIZE];

    char *token = strtok(userInput, " ");
    while (token != NULL) {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (i > 0) {
        if (strcmp(args[0], "setenv") == 0) {
            if (i != 3) {
                fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
                return;
            }
            if (setenv(args[1], args[2], 1) != 0) {
                perror("setenv");
            }
        } else if (strcmp(args[0], "unsetenv") == 0) {

            if (i != 2) {
                fprintf(stderr, "Usage: unsetenv VARIABLE\n");
                return;
            }
            if (unsetenv(args[1]) != 0) {
                perror("unsetenv");
            }
        } else if (strcmp(args[0], "cd") == 0) {

            if (i == 1) {

                char *homeDir = getenv("HOME");
                if (homeDir == NULL) {
                    fprintf(stderr, "HOME environment variable not set\n");
                } else {
                    if (chdir(homeDir) != 0) {
                        perror("chdir");
                    }
                }
            } else if (strcmp(args[1], "-") == 0) {
                char *prevDir = getenv("OLDPWD");
                if (prevDir == NULL) {
                    fprintf(stderr, "OLDPWD environment variable not set\n");
                } else {
                    if (chdir(prevDir) != 0) {
                        perror("chdir");
                    }
                }
            } else {
                if (chdir(args[1]) != 0) {
                    perror("chdir");
                }
            }

            if (getcwd(currentDir, sizeof(currentDir)) != NULL) {
                if (setenv("PWD", currentDir, 1) != 0) {
                    perror("setenv");
                }
            } else {
                perror("getcwd");
            }
        } else if (strcmp(args[0], "exit") == 0) {
            exit(0);
        } else {
            printf("Executing command: %s\n", args[0]);
        }
    }
}

int main() {
    char userInput[MAX_BUFFER_SIZE];

    while (1) {
        printf("MyShell> ");
        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            break;
        }

        userInput[strcspn(userInput, "\n")] = '\0';

        executeCommand(userInput);
    }

    return 0;
}
