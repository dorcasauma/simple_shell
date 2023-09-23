#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

#define MAX_PATH_LEN 256
/**
 * main - The main function is the entry point of the program.
 *         creating a simple shell
 * Return: The function returns 0 to indicate successful program execution.
 */
int main(void)
{
char *userInput = NULL;
char *token;
int argCount = 0;
size_t userInputSize = 0;
char *envp[] = {NULL};
int status;
pid_t my_pid;
ssize_t bytesRead;
char **shellArgs = malloc(4 * sizeof(char *));
if (shellArgs == NULL)
{
perror("malloc");
exit(EXIT_FAILURE);
}
while (1)
{
argCount = 0;
bytesRead = getline(&userInput, &userInputSize, stdin);
if (bytesRead == -1)
{
cleanupAndExit(shellArgs);
}
userInput[bytesRead - 1] = '\0';
removeLeadingSpaces(userInput);
removeTrailingSpaces(userInput);

token = strtok(userInput, " ");
while (token != NULL) {
    shellArgs[argCount++] = token;
    token = strtok(NULL, " ");
}

if (strcmp(shellArgs[0], "exit") == 0)
{
cleanupAndExit(shellArgs);
}

if (strcmp(shellArgs[0], "setenv") == 0)
{
setenvCommand(shellArgs);
continue;
}

if (strcmp(shellArgs[0], "unsetenv") == 0)
{
unsetenvCommand(shellArgs);
continue;
}

if (strchr(shellArgs[0], '/') == NULL) {
    char command_path[MAX_PATH_LEN];
    snprintf(command_path, sizeof(command_path), "/bin/%s", shellArgs[0]);
    shellArgs[0] = command_path;
}

if (userInput[0] == '\0'){
continue;
}
if (strcmp(shellArgs[0], "/bin/env") == 0)
{
printEnvironment();
continue;
}

if (access(shellArgs[0], X_OK) != 0) {
    fprintf(stderr, "Error: Command '%s' does not exist or is not executable.\n", shellArgs[0]);
    continue;
}

my_pid = fork();
if (my_pid == -1)
{
perror("fork");
cleanupAndExit(shellArgs);
}
if (my_pid == 0)
{
executeCommand(shellArgs[0], shellArgs, envp);
}
else
{
waitpid(my_pid, &status, 0);
}
}
cleanupAndExit(shellArgs);
return (0);
}
/**
 * executeCommand - The Function to execute a shell command.
 * @shellCommand: The path to the shell command.
 * @shellArgs: An array of strings representing the command and its arguments.
 * @envp: An array of environment variables.
 * Return: The function returns 0 to indicate successful program execution.
 */

void executeCommand(char *shellCommand, char **shellArgs, char *envp[]) {
    // Create a copy of shellArgs with proper null-termination
    int argCount = 0;
    int i;
    char **argsCopy;

    while (shellArgs[argCount] != NULL) {
        argCount++;
    }

    argsCopy = malloc((argCount + 1) * sizeof(char *));
    if (argsCopy == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < argCount; i++) {
        argsCopy[i] = strdup(shellArgs[i]);
        if (argsCopy[i] == NULL) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
    }
    argsCopy[argCount] = NULL;  // Null-terminate the array

    if (execve(shellCommand, argsCopy, envp) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }

    // Free the copied arguments
    for (i = 0; i < argCount; i++) {
        free(argsCopy[i]);
    }
    free(argsCopy);
}
/**
 * cleanupAndExit - The Function to clean up resources and exit
 * @userInput: what the user enters to stdin
 * @shellArgs: An array of strings representing the command and its arguments.
 * Return: The function returns 0 to indicate successful program execution.
 */
void cleanupAndExit(char **shellArgs)
{
int existStatus = 0;
if (shellArgs != NULL && shellArgs[1] != NULL) {
    existStatus = atoi(shellArgs[1]);
}
exit(existStatus);
}

void removeLeadingSpaces(char *str) {
    int len = strlen(str);
    int i, j = 0;
    int leadingSpace = 1;

    if (str == NULL) {
        return;
    }

    for (i = 0; i < len; i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r') {
            leadingSpace = 0;
        }

        if (!leadingSpace) {
            str[j++] = str[i];
        }
    }

    str[j] = '\0';
}

void removeTrailingSpaces(char *str) {
    int len = strlen(str);
    int i = len - 1;

    if (str == NULL) {
        return;
    }

    while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r')) {
        str[i] = '\0';
        i--;
    }
}

void printEnvironment() {
    extern char **environ;

    char **env;
    for (env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

void printStringArray(char **array) {
    int i;
    for (i = 0; array[i] != NULL; i++) {
        printf("shellArgs[%d]: %s\n", i, array[i]);
    }
}

void setenvCommand(char **shellArgs){
    if (shellArgs != NULL && shellArgs[1] != NULL && shellArgs[2] != NULL){
        setenv(shellArgs[1], shellArgs[2], 1);
    }
}

void unsetenvCommand(char **shellArgs){
    if (shellArgs != NULL && shellArgs[1] != NULL){
        unsetenv(shellArgs[1]);
    }
}
