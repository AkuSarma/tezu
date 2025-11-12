/*
    TCP Server Program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char msg[1024];

    // 1. Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Define server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Any local IP
    address.sin_port = htons(PORT);

    // 3. Bind socket to IP/Port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 4. Listen for client
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    // 5. Accept client connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 6. Communicate
    read(new_socket, buffer, 1024);
    printf("Client: %s\n", buffer);

    printf("Enter message to client: ");
    fgets(msg, sizeof(msg), stdin);
    send(new_socket, msg, strlen(msg), 0);

    printf("Message sent to client.\n");

    close(new_socket);
    close(server_fd);
    return 0;
}
