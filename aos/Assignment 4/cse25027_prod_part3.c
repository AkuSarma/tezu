#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>

#define BUFFER_SIZE 20
#define NUM_ITEMS 50

void sem_wait(int semid, int semnum)
{
    struct sembuf sb = {semnum, -1, 0};
    semop(semid, &sb, 1);
}

void sem_signal(int semid, int semnum)
{
    struct sembuf sb = {semnum, 1, 0};
    semop(semid, &sb, 1);
}

struct SharedData
{
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
    int SUM;
    int producedCount;
};

void producer(struct SharedData *shared, int semid, int id, int m)
{
    for (int i = 1; i <= NUM_ITEMS; i++)
    {
        sem_wait(semid, 1);
        sem_wait(semid, 0);

        shared->buffer[shared->in] = i;
        shared->in = (shared->in + 1) % BUFFER_SIZE;
        shared->count++;
        printf("Producer %d produced %d (count=%d)\n", id, i, shared->count);

        sem_signal(semid, 0);
        sem_signal(semid, 2);
        sleep(1);
    }

    sem_wait(semid, 0);
    shared->producedCount++;
    sem_signal(semid, 0);

    printf("Producer %d finished.\n", id);
    exit(0);
}

void consumer(struct SharedData *shared, int semid, int id, int m)
{
    while (1)
    {
        sem_wait(semid, 2);
        sem_wait(semid, 0);

        if (shared->producedCount == m && shared->count == 0)
        {
            sem_signal(semid, 0);
            sem_signal(semid, 2); 
            break;
        }

        int item = shared->buffer[shared->out];
        shared->out = (shared->out + 1) % BUFFER_SIZE;
        shared->count--;
        shared->SUM += item;
        printf("Consumer %d consumed %d (SUM=%d)\n", id, item, shared->SUM);

        sem_signal(semid, 0);
        sem_signal(semid, 1);
        sleep(1);
    }

    printf("Consumer %d exiting.\n", id);
    exit(0);
}

int main()
{
    int m, n;
    printf("Enter number of producers (m): ");
    scanf("%d", &m);
    printf("Enter number of consumers (n): ");
    scanf("%d", &n);

    int shm_id = shmget(IPC_PRIVATE, sizeof(struct SharedData), IPC_CREAT | 0666);
    if (shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }

    struct SharedData *shared = (struct SharedData *)shmat(shm_id, NULL, 0);
    if (shared == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    shared->in = shared->out = shared->count = shared->SUM = shared->producedCount = 0;

    int sem_id = semget(IPC_PRIVATE, 3, IPC_CREAT | 0666);
    if (sem_id < 0)
    {
        perror("semget");
        exit(1);
    }

    semctl(sem_id, 0, SETVAL, 1);
    semctl(sem_id, 1, SETVAL, BUFFER_SIZE); 
    semctl(sem_id, 2, SETVAL, 0);

    for (int i = 0; i < m; i++)
    {
        if (fork() == 0)
            producer(shared, sem_id, i + 1, m);
    }

    for (int i = 0; i < n; i++)
    {
        if (fork() == 0)
            consumer(shared, sem_id, i + 1, m);
    }

    for (int i = 0; i < m + n; i++)
        wait(NULL);

    printf("\nAll producers and consumers finished.\n");
    printf("Final SUM = %d\n", shared->SUM);
    printf("Expected SUM = %d\n", m * 25 * 51);

    shmdt(shared);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID, 0);

    return 0;
}
