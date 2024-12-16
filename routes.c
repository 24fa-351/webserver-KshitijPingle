// Add a header later

#include <stdio.h>
#include <stdlib.h>

#include "http_message.h"
#include "routes.h"

// Function to return a properly formatted HTML document
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
