#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
// Windows-specific implementation
#include <windows.h>

int main(int argc, char *argv[])
{
    // Child case: launched with --child <parentPid>
    if (argc >= 2 && strcmp(argv[1], "--child") == 0)
    {
        unsigned long parentPid = 0;
        if (argc >= 3) parentPid = strtoul(argv[2], NULL, 10);

        printf("Child: PID = %lu, Parent PID = %lu\n", (unsigned long)GetCurrentProcessId(), parentPid);
        Sleep(3000); // milliseconds
        printf("Child: Finished work.\n");
        return 0;
    }

    // Parent: create child process (same executable) and pass parent PID
    DWORD parentPid = GetCurrentProcessId();
    char cmdLine[512];
    snprintf(cmdLine, sizeof(cmdLine), "\"%s\" --child %lu", argv[0], (unsigned long)parentPid);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL,   // lpApplicationName
                       cmdLine, // lpCommandLine (writable)
                       NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        fprintf(stderr, "CreateProcess failed (error %lu)\n", GetLastError());
        return 1;
    }

    printf("Parent: PID = %lu waiting for child %lu\n", (unsigned long)parentPid, (unsigned long)pi.dwProcessId);

    // Wait for the child to exit
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 0;
    if (!GetExitCodeProcess(pi.hProcess, &exitCode))
        fprintf(stderr, "GetExitCodeProcess failed (error %lu)\n", GetLastError());

    printf("Parent: Child process finished. Status = %lu\n", (unsigned long)exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

#else
// POSIX implementation — keep original behavior
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> // for wait()

int main()
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        printf("Error: fork() failed.\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child: PID = %d, Parent PID = %d\n", getpid(), getppid());
        sleep(3);
        printf("Child: Finished work.\n");
        exit(0);
    }
    else
    {
        // Parent process waits for the child
        printf("Parent: PID = %d waiting for child %d\n", getpid(), pid);
        wait(&status); // parent waits until child exits
        printf("Parent: Child process finished. Status = %d\n", WEXITSTATUS(status));
    }

    return 0;
}
#endif
