#pragma once

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <string.h>
#include <arpa/inet.h>
#include "error_return.h"
#include "get.h"
#include "send.h"
using namespace std;

void do_http_response(int client_sock, const char* path, char* name)
{
	int ret = 0;
	FILE* resource = NULL;

	resource = fopen(path, "r");

	if (resource == NULL)
	{
		not_found(client_sock);
		return;
	}

	//����ͷ
	ret = headers(client_sock, resource, name);

	if (!ret)
	{
		//�����ļ�
		cat(client_sock, resource);
	}

	fclose(resource);
}

void* do_http_request(void* pclient_sock)
{
	int len = 0;
	char buf[256];
	char method[64];
	char url[256];
	char path[512];
	int client_sock = *(int*)pclient_sock;


	struct stat st;

	//��ȡhttp����
	//1.��ȡ������
	len = get_line(client_sock, buf, sizeof(buf));

	if (len > 0)
	{
		int i = 0, j = 0;
		while (!isspace(buf[j]) && (i < sizeof(method) - 1))
		{
			method[i] = buf[j];
			i++;
			j++;
		}
		method[i] = '\0';

		//�жϷ����Ƿ�Ϸ�
		if (strncasecmp(method, "GET", i) == 0)
		{
			printf("requst = %s\n", method);
			while (isspace(buf[++j]));
			i = 0;
			while (!isspace(buf[j]) && (i < sizeof(url) - 1))
			{
				url[i] = buf[j];
				i++;
				j++;
			}
			url[i] = '\0';
			printf("url : %s\n", url);
			//index.html => html_path
			sprintf(path, "./html_docs%s", url);

			if (path[strlen(path) - 1] == '/') {
				strcat(path, "index.html");
			}
			printf("path: %s\n", path);
			do {
				len = get_line(client_sock, buf, sizeof(buf));
				printf("read line: %s\n", buf);
			} while (len > 0);

			//��λ����������html�ļ�

			//��λurl�е�?
			char* pos = strchr(url, '?');
			if (pos)
			{
				*pos = '\0';
				if (debug) printf("real url %s\n", url);
			}


			sprintf(path, "./html_docs%s", url);
			if (debug) printf("path %s\n", path);

			//ִ��http��Ӧ

			if (stat(path, &st) == -1) // ·�������ڻ����
			{
				fprintf(stderr, "stat %s failed. reason: %s\n", path, strerror(errno));
				not_found(client_sock); //404ҳ�淵��
			}
			else   //·������
			{
				char* name = url;
				//�ж��Ƿ���Ŀ¼
				if (S_ISDIR(st.st_mode))
				{
					strcat(path, "index.html");
					name = (char*)"index.html";
					if (debug) printf("finally path: %s\n", path);
				}
				do_http_response(client_sock, path, name); //������������
			}
		}
		else
		{
			//��������
			if (debug) fprintf(stderr, "warning! other requst [%s]\n", method);//�����¼

			//ֻ��ȡhttp����ͷ�������κδ���
			do {
				len = get_line(client_sock, buf, sizeof(buf));
				printf("read line: %s\n", buf);
			} while (len > 0);

			unimplemented(client_sock);
		}
	}
	else // ������
	{
		bad_request(client_sock);
	}
	close(client_sock);
	if (pclient_sock) free(pclient_sock); //�ͷŶ�̬������ڴ�

	return NULL;
}

