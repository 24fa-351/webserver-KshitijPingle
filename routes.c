// Kshitij Pingle
// kpingle@csu.fullerton.edu
// Date: November 10, 2024
// Description: File to handle HTTP routes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_message.h"
#include "routes.h"

// Function to check if a file exists given the file's path
int file_exists(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

// Function to give back a binary file
void give_binary_file(const char *file_path)
{

    return;
}

// Function to return a the current server stats in HTML format
const char *get_stats_html(int num_requests, int received_bytes, int sent_bytes)
{
    static char html[1024];
    snprintf(html, sizeof(html),
             "<html>"
             "<head><title>Server Stats</title></head>"
             "<body>"
             "<h1>Server Statistics</h1>"
             "<p>Number of requests received: %d</p>"
             "<p>Total received bytes: %d</p>"
             "<p>Total sent bytes: %d</p>"
             "</body>"
             "</html>",
             num_requests, received_bytes, sent_bytes);
    return html;
}

// Function to handle the "/calc" route
const char *handle_calc_route(const char *query_string)
{
    static char response[256];
    int a = 0, b = 0;
    sscanf(query_string, "a=%d&b=%d", &a, &b);
    int sum = a + b;
    snprintf(response, sizeof(response),
             "<html>"
             "<head><title>Calculation Result</title></head>"
             "<body>"
             "<h1>Calculation Result</h1>"
             "<p>%d + %d = %d</p>"
             "</body>"
             "</html>",
             a, b, sum);
    return response;
}
