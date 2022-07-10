#pragma once

#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>

#include "handle.h"

using namespace std;

//“Ï≥£¥¶¿Ì
void bad_request(int client_sock); //400
void not_found(int client_sock); //404
void inner_error(int client_sock); // 500
void unimplemented(int client_sock); // 501