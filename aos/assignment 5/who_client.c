#include "rpc_who.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <server_host>\n", argv[0]);
        return 1;
    }

    CLIENT *cl = clnt_create(argv[1], WHO_PROG, WHO_VERS, "udp");
    if (cl == NULL)
    {
        clnt_pcreateerror("Error creating client");
        return 1;
    }

    who_list *result = who_proc_1(NULL, cl);

    if (result == NULL)
    {
        clnt_perror(cl, "RPC call failed");
        return 1;
    }

    printf("USER\tTTY\tHOST\tLOGIN_TIME\n");
    for (int i = 0; i < result->entries.entries_len; i++)
    {
        who_entry e = result->entries.entries[i];
        printf("%s\t%s\t%s\t%ld\n",
               e.user, e.tty, e.host, e.login_time);
    }

    clnt_destroy(cl);
    return 0;
}
