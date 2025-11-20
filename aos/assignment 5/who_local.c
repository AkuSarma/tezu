#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    struct utmp entry;
    int fd = open(UTMP_FILE, O_RDONLY);

    if (fd == -1) {
        perror("open");
        exit(1);
    }

    printf("Logged-in users:\n");

    while (read(fd, &entry, sizeof(entry)) == sizeof(entry)) {
        if (entry.ut_type == USER_PROCESS) {
            printf("%s\t%s\t%s\n",
                   entry.ut_user,
                   entry.ut_line,
                   entry.ut_host);
        }
    }

    close(fd);
    return 0;
}
