/*
CSE25027
Assignment 2 Server
Aku Sarma
cse25027@tezu.ac.in
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUF_SIZE 1024

int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t lock;

void *handle_client(void *arg)
{
    int thread_id = *((int *)arg);
    char buffer[BUF_SIZE];
    int client_fd;

    while (1)
    {
        pthread_mutex_lock(&lock);
        if (client_count > 0)
        {
            client_fd = client_sockets[--client_count];
            pthread_mutex_unlock(&lock);

            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer));
            printf("Thread %d received: %s\n", thread_id, buffer);

            char response[BUF_SIZE];
            snprintf(response, sizeof(response), "Hi, responding with Thread%d\n", thread_id);
            write(client_fd, response, strlen(response));

            close(client_fd);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            sleep(1);
        }
    }

    return NULL;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    pthread_t threads[MAX_CLIENTS];
    int thread_ids[MAX_CLIENTS];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(1);
    }

    // Listen for connections
    listen(server_fd, MAX_CLIENTS);
    printf("Server listening on port %d\n", PORT);

    pthread_mutex_init(&lock, NULL);

    // Create worker threads
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, handle_client, &thread_ids[i]);
    }

    // Accept clients
    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            continue;
        }

        pthread_mutex_lock(&lock);
        if (client_count < MAX_CLIENTS)
        {
            client_sockets[client_count++] = new_socket;
            printf("Accepted client %d\n", client_count);
        }
        else
        {
            printf("Too many clients. Connection refused.\n");
            close(new_socket);
        }
        pthread_mutex_unlock(&lock);
    }

    close(server_fd);
    return 0;
}
