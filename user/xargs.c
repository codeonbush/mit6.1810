#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// int
// main(int argc, char *argv[])
// {
//     if(argc < 2){
//         fprintf(2, "xargs error \n");
//         exit(1);
//     }
//     //区分两个命令
//     char *first_commond[MAXARG];
//     for (int i = 0; i < ; i++)
//     {
//         /* code */
//     }
    
//     exit(0);
// }

#define MAX_ARGS 100

int main(int argc, char *argv[]) {
    char *cmd[MAX_ARGS];
    char buf[1024];
    int i;

    // Make sure we have at least one argument (the command to run)
    if (argc < 2) {
        fprintf(stderr, "Usage: %s cmd [args...]\n", argv[0]);
        exit(1);
    }

    // Copy the command and its arguments into an array
    for (i = 0; i < argc - 1 && i < MAX_ARGS - 1; i++) {
        cmd[i] = argv[i + 1];
    }
    cmd[i] = NULL;

    // Read lines from standard input and execute the command for each line
    while (fgets(buf, sizeof(buf), stdin)) {
        // Remove trailing newline
        buf[strcspn(buf, "\n")] = '\0';

        // Append the line to the command's arguments
        cmd[i++] = buf;
        cmd[i] = NULL;

        // Execute the command with its new arguments
        if (fork() == 0) {
            execvp(cmd[0], cmd);
            perror("execvp");
            exit(1);
        }

        // Wait for the child process to finish
        wait(NULL);

        // Remove the line from the command's arguments
        cmd[--i] = NULL;
    }

    return 0;
}