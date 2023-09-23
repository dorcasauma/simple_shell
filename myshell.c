#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    char userInput[MAX_BUFFER_SIZE];
    char currentDir[MAX_BUFFER_SIZE];
    char previousDir[MAX_BUFFER_SIZE];

    while (1) {
        printf("MyShell> ");
        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            break;
        }


        userInput[strcspn(userInput, "\n")] = '\0';

        if (getcwd(previousDir, sizeof(previousDir)) == NULL) {
            perror("getcwd");
            continue;
        }


        if (strncmp(userInput, "cd", 2) == 0) {
            char *args = userInput + 2;

            while (*args == ' ' || *args == '\t') {
                args++;
            }
            if (*args == '\0') {
                args = getenv("HOME");
            } else if (strcmp(args, "-") == 0) {

                args = getenv("OLDPWD");
                if (args == NULL) {
                    fprintf(stderr, "OLDPWD environment variable not set\n");
                    continue;
                }
            }


            if (chdir(args) != 0) {
                perror("chdir");
                continue;
            }

            if (getcwd(currentDir, sizeof(currentDir)) == NULL) {
                perror("getcwd");
                continue;
            }
            if (setenv("PWD", currentDir, 1) != 0) {
                perror("setenv");
            }
            if (setenv("OLDPWD", previousDir, 1) != 0) {
                perror("setenv");
            }
        } else {

            printf("Executing command: %s\n", userInput);
        }
    }

    return 0;
}
