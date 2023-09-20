#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
/**
 * executeCommand: This function is responsible
 * for executing a shell command using
 * the execve system call. It will replace
 * the current process with the specified
 * shell command. If the execution fails,
 * it will print an error message and exit
 * with a failure status
 *
 * @shellCommand: The path to the shell command.
 * @shellArgs: An array of strings representing the
 * command and its arguments.
 * @envp: An array of environment variables.
 *
 */
void executeCommand(char *shellCommand, char **shellArgs, char *envp[]);
/**
 * cleanupAndExit - This function is responsible for freeing
 * allocated memory and exiting
 * the program. It is called when the program
 * needs to exit, either due to an error
 * or the "exit" command.
 *
 * @userInput: The user input buffer.
 * @shellArgs: An array of strings representing the shell command and its arguments.
 *
 */
void cleanupAndExit(char *userInput, char **shellArgs);
/**
 * main - The main function is the entry point of the program.
 *         creating a simple shell
 * @shellCommand: The path to the shell command.
 * @shellArgs: An array of strings representing the command and its arguments.
 * @envp: An array of environment variables.
 * Return: The function returns 0 to indicate successful program execution.
 */
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
