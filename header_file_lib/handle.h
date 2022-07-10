#pragma once

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "get.h"
#include "send.h"
#include "error_response.h"

static int debug = 1; //µ÷ÊÔ¿ª¹Ø

using namespace std;

void* do_http_request(void* pclient_sock);
void do_http_response(int client_sock, string path, string name);
void do_error_response(int client_sock, string path, string kind);