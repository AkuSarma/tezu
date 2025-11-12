/*
 * remote.x — RPC interface for “who” service
 */
program WHO_PROG {
    version WHO_VERS {
        string GET_USERS() = 1;
    } = 1;
} = 0x23451111;
