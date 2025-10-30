#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);

    pthread_t *threads = malloc(sizeof(pthread_t) * N);

    int *indices = malloc(sizeof(int) * N);

    void *thread_func(void *arg) {
        int k = *(int *)arg;
        printf("Thread %d done\n", k);
        pthread_exit(NULL);
    }

    for (int i = 0; i < N; ++i) {
        indices[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_func, &indices[i]) != 0) {
            perror("pthread_create");
            free(indices);
            free(threads);
            return 1;
        }
    }

    pthread_exit(NULL);

    return 0;
}