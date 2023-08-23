import os
import sys

def prompt():
    sys.stdout.write("$ ")
    sys.stdout.flush()

def main():
    while True:
        prompt()
        try:
            command = input()
            if not command:
                continue
        except EOFError:
            print()  # Print a newline when Ctrl+D is pressed
            break
        
        pid = os.fork()
        
        if pid == 0:  # Child process
            try:
                os.execve(command, [command], os.environ)
            except FileNotFoundError:
                print(f"Command not found: {command}")
            except Exception as e:
                print(f"An error occurred: {e}")
            sys.exit()
        else:
            os.waitpid(pid, 0)  # Wait for the child process to finish

if __name__ == "__main__":
    main()
