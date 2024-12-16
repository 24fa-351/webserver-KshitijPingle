#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

#include <stdbool.h>

// Variables to keep track of requests and bytes
static int num_requests = 0;
static int received_bytes = 0;
static int sent_bytes = 0;

typedef struct msg
{
    char *method;
    char *path;
    char *http_version;
    char *headers;
    char *body;
    int body_length;
} http_client_message_t;

typedef enum
{
    BAD_REQUEST,
    CLOSED_CONNECTION,
    MESSAGE
} http_read_result_t;

bool is_complete_http_message(char *buffer);

void read_http_request(int client_fd, http_client_message_t **msg, http_read_result_t *result);

int respond_to_http_request(int client_fd, http_client_message_t *http_msg);

void http_free_message(http_client_message_t *msg);

#endif // HTTP_MESSAGE_H