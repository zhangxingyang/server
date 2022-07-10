#include <iostream>
#include <limits.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "header_file_lib/handle.h"

#define SERVER_PORT 80


using namespace std;


int main() {
	int sock;
	struct sockaddr_in server_addr;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);

	bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

	printf("等待用户连接\n");

	listen(sock, INT_MAX);

	while (true)
	{
		struct sockaddr_in client;
		int client_sock = 0;
		char client_ip[256];
		pthread_t id;
		int* pclient_sock = NULL;

		socklen_t client_addr_len;
		client_addr_len = sizeof(client);
		client_sock = accept(sock, (struct sockaddr*)&client, &client_addr_len);

		printf("client ip: %s\t port : %d\n", inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)), ntohs(client.sin_port));
		pclient_sock = (int*)malloc(sizeof(int));
		*pclient_sock = client_sock;
		pthread_create(&id, NULL, do_http_request, pclient_sock);
	}
	return 0;
}
