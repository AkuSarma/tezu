/*
    UDP Client Program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd;
    char buffer[1024];
    char msg[1024];
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);

    // 1. Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Server address setup
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // 3. Send message to server
    printf("Enter message to server: ");
    fgets(msg, sizeof(msg), stdin);
    sendto(sockfd, msg, strlen(msg), 0, (const struct sockaddr *)&servaddr, len);

    // 4. Receive response
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;
}
