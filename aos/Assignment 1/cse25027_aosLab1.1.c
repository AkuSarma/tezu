/*
CSE25027
Assignment 1.1
Aku Sarma
cse25027@tezu.ac.in
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    int status;

    printf("Parent process started (PID: %d)\n", getpid());

    pid = fork();

    switch (pid)
    {
    case -1:
        // fork failed
        perror("fork failed");
        return 1;

    case 0:
        // child process
        printf("Child process started (PID: %d, PPID: %d)\n", getpid(), getppid());
        sleep(2);
        printf("Child process exiting (PID: %d)\n", getpid());
        return 0;

    default:
        // parent process
        printf("Parent process (PID: %d) created child (PID: %d)\n", getpid(), pid);

        /* --- PART 1: Parent does NOT wait (creates zombie) --- */
        printf("Parent sleeping for 10 seconds, not waiting for child.\n");
        sleep(10); // child exits before parent → zombie

        /* --- PART 2: Parent waits for child --- */
        printf("Parent waiting for child to complete...\n");
        pid = waitpid(pid, &status, 0);
        printf("Child (PID: %d) finished. Parent resuming.\n", pid);

        printf("Parent process exiting (PID: %d)\n", getpid());
        break;
    }

    return 0;
}
