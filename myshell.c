#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PATH_LEN 256

void printStringArray(char **array) {
    int i;
    for (i = 0; array[i] != NULL; i++) {
        printf("shellArgs[%d]: %s\n", i, array[i]);
    }
}

ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream) {
    size_t buffer_size = 128;
    int c;
    char *temp;
    char *line;
    ssize_t bytesRead = 0;
    if (lineptr == NULL || n == NULL) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = (char *)malloc(buffer_size);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = buffer_size;
    }

    line = *lineptr;

    while (1) {
        c = fgetc(stream);

        if (c == EOF) {
            if (bytesRead == 0) {
                return -1;
            }
            break;
        }

        if (c == '\n') {
            line[bytesRead] = '\0';
            break;
        }

        line[bytesRead] = (char)c;
        bytesRead++;

        if ((unsigned long)bytesRead >= *n - 1) {
            buffer_size *= 2;
            temp = (char *)realloc(line, buffer_size);
            if (temp == NULL) {
                free(line);
                return -1;
            }
            line = temp;
            *lineptr = line;
            *n = buffer_size;
        }
    }

    return bytesRead;
}

int main(void) {
    char *userInput = NULL;
    size_t userInputSize = 0;

    while (1) {
        ssize_t bytesRead = custom_getline(&userInput, &userInputSize, stdin);

        if (bytesRead == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }

        free(userInput);
    }

    return 0;
}
