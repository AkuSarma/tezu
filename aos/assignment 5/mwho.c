#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    struct utmp current_record;
    int utmpfd;

    utmpfd = open(UTMP_FILE, O_RDONLY);
    if (utmpfd == -1) {
        perror("Could not open UTMP file");
        return 1;
    }

    printf("USER\tTTY\tHOST\tLOGIN_TIME\n");

    while (read(utmpfd, &current_record, sizeof(current_record)) == sizeof(current_record)) {
        if (current_record.ut_type == USER_PROCESS) {
            printf("%s\t%s\t%s\t%ld\n",
                current_record.ut_user,
                current_record.ut_line,
                current_record.ut_host,
                (long) current_record.ut_tv.tv_sec);
        }
    }

    close(utmpfd);
    return 0;
}
