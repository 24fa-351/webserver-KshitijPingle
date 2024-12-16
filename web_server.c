// Kshitij Pingle
// kpingle@csu.fullerton.edu
// Date: 15 December, 2024
// Description: Program to simulate a web server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "http_message.h"

#define MAX_CONNECTIONS 5

// Input pointer is freed in this function
void handle_connection(int *client_fd_ptr)
{
    int client_fd = *client_fd_ptr;
    free(client_fd_ptr);

    http_client_message_t *http_msg;
    http_read_result_t result;

    while (1)
    {
        // Equivalent of reading from the client from echo server assignment
        read_http_request(client_fd, &http_msg, &result);

        if (result == BAD_REQUEST)
        {
            printf("Bad request.\n");
            close(client_fd);
            break;
        }
        else if (result == CLOSED_CONNECTION)
        {
            printf("Connection closed.\n");
            close(client_fd);
            return;
        }

        int pid = fork();
        if (pid == 0)
        {
            // Equivalent of writing to the client from echo server assignment
            respond_to_http_request(client_fd, http_msg);

            http_free_message(http_msg);
        }

        wait(NULL);
    }

    printf("Done with connection on %d.\n", client_fd);
}

int main(int argc, char *argv[])
{
    int PORT = 8000; // Random port number

    // Verify arguments
    if ((argc > 3))
    {
        printf("Incorrect number of arguments.\n");
        printf("Usage: %s <-p>(optional) <port_number>(optional)\n", argv[0]);
        exit(1);
    }

    if ((argc > 1) && (strcmp(argv[1], "-p") != 0))
    {
        printf("Wrong arguments passed.\n");
        printf("2nd optional argument should be '-p' and 3rd argument should be a port number.\n");
        printf("Usage: %s <-p>(optional) <port_number>(optional)\n", argv[0]);
        exit(1);
    }

    if (argc == 2)
    {
        printf("Incorrect number of arguments.\n");
        printf("Please also provide a port number if you use the optional '-p' argument.\n");
        printf("Usage: %s <-p>(optional) <port_number>(optional)\n", argv[0]);
        exit(1);
    }

    if (argc == 3)
    {
        PORT = atoi(argv[2]);
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET = IPv4
    // SOCK_STREAM = TCP
    // 0 = protocol for IP (Internet Protocol)

    // NOTE : Check if this is the correct sock struct to use for bind()
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;         // AF_INET = IPv4
    sock_addr.sin_port = htons(PORT);       // htons() = host to network short
    sock_addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY = any IP address

    int returnval = 0;

    // Bind socket to port
    // Requires sending pointer to sockaddr_in struct typecasted to sockaddr
    returnval = bind(server_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));

    if (returnval < 0)
    {
        printf("Error binding socket to port %d.\n", PORT);
        exit(1);
    }

    returnval = listen(server_fd, MAX_CONNECTIONS);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (1)
    {
        int *client_fd_buf = malloc(sizeof(int));

        // Await for a connection on a server_fd. When a connection arrives, open a new socket to communicate with it
        *client_fd_buf = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

        printf("Connection established on %d.\n", *client_fd_buf);

        // Make a thread to handle the connection
        pthread_t thread;

        pthread_create(&thread, NULL, (void *(*)(void *))handle_connection, (void *)client_fd_buf);
        printf("Done with connection on %d.\n", *client_fd_buf);
    }

    return 0;
}