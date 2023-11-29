#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

// Function to parse the input line and populate the buffer
int parser(char** buf, char* input_line) {
    int i = 0;
    char line[PATH_MAX];
    char* buffer;

    // Copy input_line to line, removing the newline character
    while (input_line[i] != '\n' && i < PATH_MAX - 1) {
        line[i] = input_line[i];
        i++;
    }
    line[i] = '\0';

    // Tokenize the line into buf using space as a delimiter
    buffer = strtok(line, " ");
    i = 0;
    while (buffer != NULL) {
        buf[i] = buffer;
        buffer = strtok(NULL, " ");
        i++;
    }
    buf[i] = NULL;
    return i;
}

// Function to handle the 'help' command
void command_help(char* command, char* command_argument) {
    if (command_argument == NULL) {
        // Display general help information
        printf("\n***MicroShell***\n**Author:Pavel Bielko**\n*Implemented commands:exit,help,cd,pwd,echo*\n\n");
    } else {
        // Display specific help information for the provided command_argument
        if (strcmp(command_argument, "exit") == 0) {
            printf("Ending shell\n");
        } else if (strcmp(command_argument, "help") == 0) {
            printf("Display information about project\n");
        } else if (strcmp(command_argument, "cd") == 0) {
            printf("Change the working directory\n");
        } else if (strcmp(command_argument, "pwd") == 0) {
            printf("Print the name of the current directory\n");
        } else if (strcmp(command_argument, "echo") == 0) {
            printf("Write arguments to the standard output\n");
        } else {
            printf("%s%s: command is not implemented by the author\n", RED, command);
        }
    }
}

// Function to change the current working directory
int command_cd(char* command, char* command_argument, int size) {
    if (size <= 2) {
        // Check for NULL or ~ to change to the home directory
        if (command_argument == NULL || (command_argument[0] == '~' && command_argument[1] == NULL)) {
            chdir(getenv("HOME"));
        } else {
            // Change directory to the specified path
            int trigger = chdir(command_argument);
            if (trigger == -1) {
                printf("%s%s %s: No such file or directory\n", RED, command, command_argument);
            }
        }
    } else {
        // Too many arguments for cd command
        printf("%scd: too many arguments", RED);
    }
}

// Function to handle the 'echo' command
void command_echo(char* out_line) {
    char output_line[PATH_MAX];
    int call_index = 4, output_index = 0;  // Start from the 4th character

    // Copy characters from out_line to output_line, excluding the command 'echo'
    while (out_line[call_index] != '\n') {
        output_line[output_index] = out_line[call_index];
        call_index++;
        output_index++;
    }
    output_line[output_index] = '\0';  // Terminate the output line
    printf("%s\n", output_line);
}

// Function to print the current working directory
void command_pwd(char* current_dir) {
    printf("%s\n", current_dir);
}

int main(void) {
    char line[PATH_MAX];
    char *buf[BUFSIZ];
    int buf_size = 0;

    char PathName[PATH_MAX];
    char *current_path;
    char history[2][PATH_MAX];

    // Display initial information about the microshell
    printf("\n***MicroShell***\n**Author:Pavel Bielko**\n*Implemented commands:exit,help,cd,pwd,echo*\n\n");

    for (;;) {
        // Save the current path in history
        current_path = getcwd(PathName, PATH_MAX);
        strcpy(history[0], history[1]);
        strcpy(history[1], current_path);

        // Display the shell prompt
        printf("%s%s%s[%s]\n%s$ ", RED, getenv("LOGNAME"), BLUE, current_path, RESET);
        fgets(line, BUFSIZ, stdin);

        // Check for an empty command
        int is_empty_or_spaces = 1;
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] != ' ' && line[i] != '\n' && line[i] != '\t') {
                is_empty_or_spaces = 0;
                break;
            }
        }

        if (is_empty_or_spaces) {
            printf("Empty command or contains only spaces. Please enter a valid command.\n");
            continue;
        }

        buf_size = parser(buf, line);

        // Execute the appropriate command based on user input
        if (strcmp(buf[0], "exit") == 0) {
            return 0;
        } else if (strcmp(buf[0], "help") == 0) {
            command_help(buf[0], buf[1]);
        } else if (strcmp(buf[0], "cd") == 0) {
            if (buf[1] != NULL && strcmp(buf[1], "-") == 0) {
                command_cd(buf[0], history[0], buf_size);
                printf("%s\n", history[0]);
            } else {
                command_cd(buf[0], buf[1], buf_size);
            }
        } else if (strcmp(buf[0], "echo") == 0) {
            command_echo(line);
        } else if (strcmp(buf[0], "pwd") == 0) {
            command_pwd(current_path);
        } else {
            // Fork a new process for external commands
            pid_t pid = fork();
            if (pid == 0) {
                execvp(buf[0], buf);
                if (errno != 0) {
                    // Display an error message if the command is not found
                    printf("%s%s: No such file or directory\n", RED, buf[0]);
                    exit(0);
                } else {
                    exit(0);
                }
            } else {
                waitpid(pid, 0, 0);
            }
        }
    }
}

