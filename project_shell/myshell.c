#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
char *userInput = NULL;
size_t userInputSize = 0;
char *shellCommand = "/bin/bash";
char *envp[] = {NULL};
int execveResult;
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
exit(EXIT_FAILURE);
}
userInput[bytesRead - 1] = '\0';
if (strcmp(userInput, "exit") == 0)
{
free(userInput);
free(shellArgs);
exit(0);
}
shellArgs[0] = shellCommand;
shellArgs[1] = "-c";
shellArgs[2] = userInput;
shellArgs[3] = NULL;
my_pid = fork();
if (my_pid == -1)
{
perror("fork");
free(userInput);
free(shellArgs);
exit(EXIT_FAILURE);
}
if (my_pid == 0)
{
execveResult = execve(shellCommand, shellArgs, envp);
perror("execve");
free(userInput);
free(shellArgs);
exit(EXIT_FAILURE);
}
else
{
waitpid(my_pid, &status, 0);
}
}
free(userInput);
free(shellArgs);
return (0);
}
