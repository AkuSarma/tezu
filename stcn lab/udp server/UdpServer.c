/*
    UDP Server Program
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
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    // 1. Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Server address setup
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // 3. Bind socket to port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    // 4. Receive message from client
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Client: %s\n", buffer);

    // 5. Send reply to client
    printf("Enter message to client: ");
    fgets(msg, sizeof(msg), stdin);
    sendto(sockfd, msg, strlen(msg), 0, (const struct sockaddr *)&cliaddr, len);

    printf("Message sent to client.\n");

    close(sockfd);
    return 0;
}
