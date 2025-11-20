#include <stdio.h>
#include <stdlib.h>
#include "remote.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <server-hostname>\n", argv[0]);
        exit(1);
    }

    CLIENT *clnt;
    char *result;

    clnt = clnt_create(argv[1], WHO_PROG, WHO_VERS, "udp");
    if (!clnt) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    result = *who_proc_1(NULL, clnt);

    if (result == NULL) {
        clnt_perror(clnt, "Call failed");
    } else {
        printf("Logged-in users (REMOTE):\n%s", result);
    }

    clnt_destroy(clnt);

    return 0;
}
