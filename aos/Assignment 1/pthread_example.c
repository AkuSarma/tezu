#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
// Windows implementation using Win32 threads
#include <windows.h>

DWORD WINAPI thread_function_win(LPVOID arg)
{
    printf("HELLO WORLD\n");
    return 0;
}

int main()
{
    DWORD threadId = 0;
    HANDLE thread = CreateThread(NULL, 0, thread_function_win, NULL, 0, &threadId);
    if (thread == NULL)
    {
        fprintf(stderr, "Error: CreateThread failed (error %lu)\n", GetLastError());
        return EXIT_FAILURE;
    }

    printf("Main thread: Created a new thread with ID %lu\n", (unsigned long)threadId);

    // Wait for the created thread to finish
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);

    printf("Main thread: Thread has finished execution.\n");
    return 0;
}

#else
// POSIX implementation using pthreads
#include <pthread.h> // For pthread functions

void *thread_function(void *arg)
{
    printf("HELLO WORLD\n");
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_id; // Declare a thread variable

    int ret = pthread_create(&thread_id, NULL, thread_function, NULL);
    if (ret != 0)
    {
        printf("Error: Unable to create thread, return code: %d\n", ret);
        exit(EXIT_FAILURE);
    }

    printf("Main thread: Created a new thread with ID %lu\n", (unsigned long)thread_id);

    // Wait for the created thread to finish
    pthread_join(thread_id, NULL);

    printf("Main thread: Thread has finished execution.\n");

    // Exit main thread safely
    pthread_exit(NULL);
    return 0;
}
#endif
