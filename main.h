/**
 * executeCommand - Execute a shell command using execve.
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
 * cleanupAndExit - Clean up resources and exit the program.
 * cleanupAndExit: This function is responsible for freeing
 * allocated memory and exiting
 * the program. It is called when the program
 * needs to exit, either due to an error
 * or the "exit" command.
 *
 * @userInput: The user input buffer.
 * @shellArgs: An array of strings representing the
 * shell command and its arguments.
 *
 */
void cleanupAndExit(char *userInput, char **shellArgs);

void removeLeadingSpaces(char *str);

void removeTrailingSpaces(char *str);
