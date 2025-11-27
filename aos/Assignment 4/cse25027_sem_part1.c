#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main()
{
    int shm_id;
    int *X;
    pid_t pid1, pid2;

    shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }

    X = (int *)shmat(shm_id, NULL, 0);
    if (X == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    *X = 0; 

    printf("Initial value of X: %d\n", *X);

    pid1 = fork();
    if (pid1 == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            int temp = *X;
            temp++;
            sleep(1); 
            *X = temp;
        }
        shmdt(X);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            int temp = *X;
            temp--;
            sleep(1);
            *X = temp;
        }
        shmdt(X);
        exit(0);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Final value of X: %d\n", *X);

    shmdt(X);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
