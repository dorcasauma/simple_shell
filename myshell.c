#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

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
cleanupAndExit(userInput, shellArgs);
}
userInput[bytesRead - 1] = '\0';
if (strcmp(userInput, "exit") == 0)
{
cleanupAndExit(userInput, shellArgs);
}
removeLeadingSpaces(userInput);
removeTrailingSpaces(userInput);

token = strtok(userInput, " ");
while (token != NULL) {
    shellArgs[argCount++] = token;
    token = strtok(NULL, " ");
}
if (userInput[0] == '\0'){
continue;
}
my_pid = fork();
if (my_pid == -1)
{
perror("fork");
cleanupAndExit(userInput, shellArgs);
}
if (my_pid == 0)
{
executeCommand(userInput, shellArgs, envp);
}
else
{
waitpid(my_pid, &status, 0);
}
}
cleanupAndExit(userInput, shellArgs);
return (0);
}
/**
 * executeCommand - The Function to execute a shell command.
 * @shellCommand: The path to the shell command.
 * @shellArgs: An array of strings representing the command and its arguments.
 * @envp: An array of environment variables.
 * Return: The function returns 0 to indicate successful program execution.
 */

void executeCommand(char *shellCommand, char **shellArgs, char *envp[])
{
if (execve(shellCommand, shellArgs, envp) == -1)
{
perror("execve");
exit(EXIT_FAILURE);
}
}
/**
 * cleanupAndExit - The Function to clean up resources and exit
 * @userInput: what the user enters to stdin
 * @shellArgs: An array of strings representing the command and its arguments.
 * Return: The function returns 0 to indicate successful program execution.
 */
void cleanupAndExit(char *userInput, char **shellArgs)
{
free(userInput);
free(shellArgs);
exit(0);
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
