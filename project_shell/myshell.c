#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
/**
 * main - The main function is the entry point of the program.
 *         creating a simple shell
 */
void executeCommand(char *shellCommand, char **shellArgs, char *envp[]);
void cleanupAndExit(char *userInput, char **shellArgs);
int main(void)
{
char *userInput = NULL;
size_t userInputSize = 0;
char *shellCommand = "/bin/bash";
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
printf("mysimpleshell $: ");
bytesRead = getline(&userInput, &userInputSize, stdin);
if (bytesRead == -1)
{
perror("getline");
cleanupAndExit(userInput, shellArgs);
}
userInput[bytesRead - 1] = '\0';
if (strcmp(userInput, "exit") == 0)
{
cleanupAndExit(userInput, shellArgs);
}
shellArgs[0] = shellCommand;
shellArgs[1] = "-c";
shellArgs[2] = userInput;
shellArgs[3] = NULL;
my_pid = fork();
if (my_pid == -1)
{
perror("fork");
cleanupAndExit(userInput, shellArgs);
}
if (my_pid == 0)
{
executeCommand(shellCommand, shellArgs, envp);
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
 * @shellArgs: An array of strings representing the command and its arguments.
 * Return: The function returns 0 to indicate successful program execution.
 */
void cleanupAndExit(char *userInput, char **shellArgs)
{
free(userInput);
free(shellArgs);
exit(0);
}
