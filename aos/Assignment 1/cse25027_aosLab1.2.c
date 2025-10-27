/*
CSE25027
Assignment 1.2
Aku Sarma
cse25027@tezu.ac.in
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    char command[80];
    char *argv[4];
    pid_t pid;

    while (1)
    {
        printf("%% ");
        fflush(stdout);

        // read command from user
        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
            continue;

        pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            exit(1);
        }
        else if (pid == 0)
        {
            argv[0] = "/bin/sh";
            argv[1] = "-c";
            argv[2] = command;
            argv[3] = (char *)NULL;

            execv("/bin/sh", argv);

            perror("execv failed");
            exit(1);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }

    printf("\nExiting myshell.\n");
    return 0;
}