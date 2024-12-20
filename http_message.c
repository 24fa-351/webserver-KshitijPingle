// Kshitij Pingle
// kpingle@csu.fullerton.edu
// Date: November 10, 2024
// Description: File to handle HTTP messages

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "http_message.h"
#include "routes.h"

bool is_complete_http_message(char *buffer)
{
    if (strlen(buffer) < 10)
    {
        return false;
    }
    if (strncmp(buffer, "GET ", 4) != 0)
    {
        return false;
    }
    if (strcmp(buffer + strlen(buffer) - 4, "\r\n\r\n") != 0)
    {
        return false;
    }

    return true;
}

void read_http_request(int client_fd, http_client_message_t **msg, http_read_result_t *result)
{
    *msg = malloc(sizeof(http_client_message_t));
    char buffer[1000];
    strcpy(buffer, "");

    // Keep reading until we get a complete HTTP message
    while (!is_complete_http_message(buffer))
    {
        int bytes_read = read(client_fd, buffer + strlen(buffer), sizeof(buffer) - strlen(buffer));
        if (bytes_read == 0)
        {
            *result = CLOSED_CONNECTION;
            return;
        }
    }

    (*msg)->method = strdup(buffer);
    *result = MESSAGE;
}

int respond_to_http_request(int client_fd, http_client_message_t *http_msg)
{
    char response[1000];
    char *body[1000];

    if (strstr("/static", http_msg->path) != NULL)
    {
        // get the path of the file
        char *path = http_msg->path + 8; // Skip the "/static/" part

        if (file_exists(path))
        {
            // give back the binary file
            // binary_file = give_binary_file(path)
        }
        else
        {
            // 404 Not Found
            sprintf(response, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
            write(client_fd, response, strlen(response));
            return 0;
        }
    }
    else if (strstr("/stats", http_msg->path) != NULL)
    {
        *body = get_stats_html(num_requests, received_bytes, sent_bytes);
    }
    else if (strstr("/calc", http_msg->path) != NULL)
    {
        // Extract query string
        char *query = strchr(http_msg->path, '?');
        if (query != NULL)
        {
            query++; // Skip the '?' character
            *body = handle_calc_route(query);
        }
    }

    // sprintf = String printf
    // Print in the string
    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", http_msg->body_length, http_msg->body);
    strcat(response, *body);
    write(client_fd, response, strlen(response));
    return 0;
}

void http_free_message(http_client_message_t *msg)
{
    free(msg->method);
    free(msg->path);
    free(msg->http_version);
    free(msg->headers);
    free(msg->body);
    free(msg);
}