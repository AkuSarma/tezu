#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>
#include "remote.h"

char **who_proc_1_svc(void *arg, struct svc_req *req) {
    static char *result = NULL;

    FILE *fp = fopen(UTMP_FILE, "rb");
    struct utmp entry;
    char buffer[4096] = "";
    char line[256];

    if (!fp) {
        result = strdup("Error opening UTMP file.");
        return &result;
    }

    while (fread(&entry, sizeof(entry), 1, fp) == 1) {
        if (entry.ut_type == USER_PROCESS) {
            snprintf(line, sizeof(line),
                     "%s\t%s\t%s\n",
                     entry.ut_user,
                     entry.ut_line,
                     entry.ut_host);
            strcat(buffer, line);
        }
    }

    fclose(fp);

    result = strdup(buffer);
    return &result;
}
