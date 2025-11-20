#include "rpc_who.h"
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

who_list * who_proc_1_svc(void *x, struct svc_req *rqstp) {
    static who_list result;

    result.entries.entries_len = 0;

    struct utmp record;
    int fd = open(UTMP_FILE, O_RDONLY);
    if (fd == -1) return &result;

    while (read(fd, &record, sizeof(record)) == sizeof(record)) {
        if (record.ut_type == USER_PROCESS) {
            who_entry *e = &result.entries.entries[result.entries.entries_len++];
            strncpy(e->user, record.ut_user, 32);
            strncpy(e->tty, record.ut_line, 32);
            strncpy(e->host, record.ut_host, 256);
            e->login_time = record.ut_tv.tv_sec;
        }
    }
    close(fd);

    return &result;
}
