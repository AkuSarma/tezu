#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>
#include "remote.h"

#define MAX_OUTPUT 4096

char **get_users_1_svc(void *arg, struct svc_req *req)
{
    static char *result;
    static char buffer[MAX_OUTPUT];
    struct utmp record;
    FILE *fp;

    memset(buffer, 0, sizeof(buffer));
    fp = fopen("/var/run/utmp", "rb");
    if (fp == NULL)
    {
        perror("Cannot open /var/run/utmp");
        result = "Error opening utmp file";
        return &result;
    }

    strcat(buffer, "USER\tTTY\tHOST\n");
    strcat(buffer, "----------------------------------\n");

    while (fread(&record, sizeof(struct utmp), 1, fp) == 1)
    {
        if (record.ut_type == USER_PROCESS)
        {
            char line[256];
            snprintf(line, sizeof(line), "%-10s\t%-10s\t%-20s\n",
                     record.ut_user,
                     record.ut_line,
                     record.ut_host);
            strcat(buffer, line);
        }
    }
    fclose(fp);

    result = buffer;
    return &result;
}
