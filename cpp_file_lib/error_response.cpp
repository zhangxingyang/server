#include "../header_file_lib/error_response.h"

void bad_request(int client_sock) { // 400
	do_error_response(client_sock, "400.html", "400 BAD REQUEST");
}

void not_found(int client_sock) { // 404
	do_error_response(client_sock, "404.html", "404 NOT FOUND");
}

void inner_error(int client_sock) { // 500
	do_error_response(client_sock, "500.html", "500 Internal Server Error");
}

void unimplemented(int client_sock) { // 501
	do_error_response(client_sock, "501.html", "501 Method Not Implemented");
}

