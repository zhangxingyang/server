#pragma once

#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>

#include "error_response.h"

using namespace std;

int headers(int client_sock, string name);
int error_headers(int client_sock, string kind);

void cat(int client_sock, string path);