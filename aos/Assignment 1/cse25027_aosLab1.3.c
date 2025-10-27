/*
CSE25027
Assignment 1.2
Aku Sarma
cse25027@tezu.ac.in
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *printMessage(void *arg)
{
    printf("Thread started: Printing message...\n");
    printf("HELLO WORLD\n");

    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_id;
    int result;

    printf("Main process started (PID: %d)\n", getpid());

    result = pthread_create(&thread_id, NULL, printMessage, NULL);

    if (result != 0)
    {
        perror("Thread creation failed");
        exit(1);
    }

    printf("Main process created a thread (TID: %lu)\n", thread_id);

    pthread_join(thread_id, NULL);

    printf("Thread finished execution.\n");
    printf("Main process exiting.\n");

    return 0;
}
