#include <stdio.h>
#include <stdlib.h>
#include "remote.h"

int main(int argc, char *argv[])
{
    CLIENT *clnt;
    char *server_host;
    char **result;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <server_host>\n", argv[0]);
        exit(1);
    }

    server_host = argv[1];

    clnt = clnt_create(server_host, WHO_PROG, WHO_VERS, "udp");
    if (clnt == NULL)
    {
        clnt_pcreateerror(server_host);
        exit(1);
    }

    result = get_users_1(NULL, clnt);
    if (result == NULL)
    {
        clnt_perror(clnt, "Call failed");
        exit(1);
    }

    printf("Logged in users from server %s:\n\n", server_host);
    printf("%s\n", *result);

    clnt_destroy(clnt);
    return 0;
}
