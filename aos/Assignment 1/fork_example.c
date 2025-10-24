#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
// Windows-specific includes
#include <windows.h>
int main(int argc, char *argv[])
{
    // If launched as child: argv[1] == "--child" and argv[2] is parent PID
    if (argc >= 2 && strcmp(argv[1], "--child") == 0)
    {
        unsigned long parentPid = 0;
        if (argc >= 3) parentPid = strtoul(argv[2], NULL, 10);

        printf("Child Process: PID = %lu, Parent PID = %lu\n", (unsigned long)GetCurrentProcessId(), parentPid);
        Sleep(2000); // milliseconds
        printf("Child Process: Execution completed.\n");
        return 0;
    }

    // Parent: create child process (same executable) and pass parent PID
    DWORD parentPid = GetCurrentProcessId();
    char cmdLine[512];
    // Quote argv[0] in case of spaces
    snprintf(cmdLine, sizeof(cmdLine), "\"%s\" --child %lu", argv[0], (unsigned long)parentPid);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL,   // lpApplicationName
                       cmdLine, // lpCommandLine (must be writable)
                       NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        fprintf(stderr, "CreateProcess failed (error %lu)\n", GetLastError());
        return 1;
    }

    printf("Parent Process: PID = %lu, Child PID = %lu\n", (unsigned long)parentPid, (unsigned long)pi.dwProcessId);
    // Wait for child to finish (child sleeps shorter)
    WaitForSingleObject(pi.hProcess, INFINITE);
    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    Sleep(0); // no-op, kept for similarity with POSIX timing choices
    printf("Parent Process: Execution completed.\n");
    return 0;
}
#else
// POSIX includes
#include <unistd.h>    // fork(), getpid(), getppid(), sleep()
#include <sys/types.h> // pid_t

int main()
{
    pid_t pid; // process ID variable

    // Create a new process
    pid = fork();

    if (pid < 0)
    {
        // fork() failed
        printf("Error: fork() unsuccessful.\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        sleep(2); // simulate work
        printf("Child Process: Execution completed.\n");
    }
    else
    {
        // Parent process
        printf("Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);
        sleep(5); // parent sleeps longer so child finishes first
        printf("Parent Process: Execution completed.\n");
    }

    return 0;
}
#endif
