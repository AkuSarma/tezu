#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

void sem_wait(int semid)
{
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_signal(int semid)
{
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

int main()
{
    int shm_id, sem_id;
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

    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (sem_id < 0)
    {
        perror("semget");
        exit(1);
    }

    semctl(sem_id, 0, SETVAL, 1);

    printf("Initial value of X: %d\n", *X);

    pid1 = fork();
    if (pid1 == 0)
    {
        for (int i = 0; i < 10; i++)
        {
            sem_wait(sem_id);
            int temp = *X;
            temp++;
            sleep(1);
            *X = temp;
            sem_signal(sem_id);
        }
        shmdt(X);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        for (int i = 0; i < 10; i++)
        {
            sem_wait(sem_id);
            int temp = *X;
            temp--;
            sleep(1);
            *X = temp;
            sem_signal(sem_id);
        }
        shmdt(X);
        exit(0);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Final value of X (with semaphore): %d\n", *X);

    shmdt(X);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID, 0);

    return 0;
}
