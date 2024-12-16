#ifndef ROUTES_H
#define ROUTES_H

#include "http_message.h"

int file_exists(const char *file_path);

void give_binary_file(const char *file_path);

const char *get_stats_html(int num_requests, int received_bytes, int sent_bytes);

const char *handle_calc_route(const char *query_string);

#endif // ROUTES_H