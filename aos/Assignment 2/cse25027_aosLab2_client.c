/*
CSE25027
Assignment 2 Client
Aku Sarma
cse25027@tezu.ac.in
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE] = "hello";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        close(sock);
        return 1;
    }

    write(sock, buffer, strlen(buffer));
    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer));

    printf("Server replied: %s", buffer);
    close(sock);
    return 0;
}
