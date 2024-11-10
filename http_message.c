// Add a header later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "http_message.h"


bool is_complete_http_message(char* buffer) {
    if (strlen(buffer) < 10) {
        return false;
    }
    if (strncmp(buffer, "GET ", 4) != 0) {
        return false;
    }
    if (strcmp(buffer + strlen(buffer) - 4, "\r\n\r\n") != 0) {
        return false;
    }

    return true;
}


void read_http_request(int client_fd, http_client_message_t** msg, http_read_result_t* result) {
    *msg = malloc(sizeof(http_client_message_t));
    char buffer[1000];
    strcpy(buffer, "");

    // Keep reading until we get a complete HTTP message
    while (!is_complete_http_message(buffer)) {
        int bytes_read = read(client_fd, buffer + strlen(buffer), sizeof(buffer) - strlen(buffer));
        if (bytes_read == 0) {
            *result = CLOSED_CONNECTION;
            return;
        }
    }

    (*msg)->method = strdup(buffer);
    *result = MESSAGE;
}

int respond_to_http_request(int client_fd, http_client_message_t* http_msg) {
    char response[1000];

    // sprintf = String printf
        // Print in the string
    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", http_msg->body_length, http_msg->body);
    write(client_fd, response, strlen(response));
    return 0;
}


void http_free_message(http_client_message_t* msg) {
    free(msg->method);
    free(msg->path);
    free(msg->http_version);
    free(msg->headers);
    free(msg->body);
    free(msg);
}