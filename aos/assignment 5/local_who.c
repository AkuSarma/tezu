#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    struct utmp record;
    FILE *fp;

    fp = fopen("/var/run/utmp", "rb");
    if (fp == NULL)
    {
        perror("Cannot open /var/run/utmp");
        exit(1);
    }

    printf("USER\t\tTTY\t\tHOST\n");
    printf("---------------------------------------------\n");

    while (fread(&record, sizeof(struct utmp), 1, fp) == 1)
    {
        if (record.ut_type == USER_PROCESS)
        {
            printf("%-10s\t%-10s\t%-20s\n",
                   record.ut_user,
                   record.ut_line,
                   record.ut_host);
        }
    }

    fclose(fp);
    return 0;
}
