#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PATH_LEN 256

void executeCommand(char *shellCommand, char **shellArgs, char *envp[]);
void cleanupAndExit(char *userInput, char **shellArgs);
void removeLeadingSpaces(char *str);
void removeTrailingSpaces(char *str);
void printEnvironment();
char *findCommandPath(const char *command);
void printShellArgs(char **shellArgs);
ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream);

int main(void)
{
    char *userInput = NULL;
    char *token;
    size_t userInputSize = 0;
    char *envp[] = {NULL};
    int status;
    pid_t my_pid;
    ssize_t bytesRead;
    size_t newline_pos;

    char **shellArgs = malloc(4 * sizeof(char *));
    if (shellArgs == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int argCount = 0;

        bytesRead = custom_getline(&userInput, &userInputSize, stdin);
        if (bytesRead == -1)
        {
            cleanupAndExit(userInput, shellArgs);
        }
        if (strcmp(userInput, "exit") == 0)
        {
            cleanupAndExit(userInput, shellArgs);
        }
        removeLeadingSpaces(userInput);
        removeTrailingSpaces(userInput);

        newline_pos = strcspn(userInput, "\n");
        userInput[newline_pos] = '\0';

        printf("userninput %s", userInput);

        token = strtok(userInput, " ");
        while (token != NULL)
        {
            shellArgs[argCount++] = token;
            token = strtok(NULL, " ");
        }

        if (strchr(shellArgs[0], '/') == NULL)
        {
            char *new_command_path = findCommandPath(shellArgs[0]);
            if (new_command_path == NULL)
            {
                printf("%s: not found\n", shellArgs[0]);
                continue;
            }
            strcpy(shellArgs[0], new_command_path);
        }

        if (userInput[0] == '\0')
        {
            continue;
        }
        if (strcmp(shellArgs[0], "/bin/env") == 0)
        {
            printEnvironment();
            continue;
        }
        if (access(shellArgs[0], X_OK) != 0)
        {
            fprintf(stderr, "Error: Command '%s' does not exist or is not executable.\n", shellArgs[0]);
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
            executeCommand(shellArgs[0], shellArgs, envp);
        }
        else
        {
            waitpid(my_pid, &status, 0);
        }
    }
    cleanupAndExit(userInput, shellArgs);
    return (0);
}

void executeCommand(char *shellCommand, char **shellArgs, char *envp[])
{
    if (execve(shellCommand, shellArgs, envp) == -1)
    {
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

void cleanupAndExit(char *userInput, char **shellArgs)
{
    free(userInput);
    free(shellArgs);
    exit(0);
}

void removeLeadingSpaces(char *str)
{
    int len = strlen(str);
    int i, j = 0;
    int leadingSpace = 1;

    for (i = 0; i < len; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
        {
            leadingSpace = 0;
        }

        if (!leadingSpace)
        {
            str[j++] = str[i];
        }
    }

    str[j] = '\0';
}

void removeTrailingSpaces(char *str)
{
    int len = strlen(str);
    int i = len - 1;

    while (i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r'))
    {
        str[i] = '\0';
        i--;
    }
}

void printEnvironment()
{
    extern char **environ;

    char **env;
    for (env = environ; *env != NULL; env++)
    {
        printf("%s\n", *env);
    }
}

char *findCommandPath(const char *command)
{
    char *path;
    char *path_copy;
    char *dir;
    path = getenv("PATH");
    path_copy = strdup(path);
    dir = strtok(path_copy, ":");
    if (path == NULL)
    {
        fprintf(stderr, "Error: PATH environment variable is not set.\n");
        return NULL;
    }

    if (path_copy == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    while (dir != NULL)
    {
        char command_path[MAX_PATH_LEN];
        snprintf(command_path, sizeof(command_path), "%s/%s", dir, command);
        if (access(command_path, X_OK) == 0)
        {
            free(path_copy);
            return strdup(command_path);
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

void printShellArgs(char **shellArgs)
{
    int i = 0;
    printf("start printing\n");
    while (shellArgs[i] != NULL)
    {
        printf("shellArgs[%d]: %s\n", i, shellArgs[i]);
        i++;
    }
    printf("end of printing\n");
}

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream)
{
    size_t buffer_size = 128;
    ssize_t characters_read = 0;
    int c;
    char *temp;
    char *line;

    if (lineptr == NULL || n == NULL)
    {
        return -1;
    }

    if (*lineptr == NULL)
    {
        *lineptr = (char *)malloc(buffer_size);
        if (*lineptr == NULL)
        {
            return -1;
        }
        *n = buffer_size;
    }

    line = *lineptr;

    while (1)
    {
        c = fgetc(stream);

        if (c == EOF)
        {
            if (characters_read == 0)
            {
                return -1;
            }
            break;
        }

        if (c == '\n')
        {
            line[characters_read] = '\0';
            break;
        }

        line[characters_read] = (char)c;
        characters_read++;

        if ((unsigned long)characters_read >= *n)
        {
            buffer_size *= 2;
            temp = realloc(line, buffer_size);
            if (temp == NULL)
            {
                return -1;
            }
            line = temp;
            *n = buffer_size;
        }
    }

    *lineptr = line;
    return characters_read;
}
