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

	//发送头
	ret = headers(client_sock, resource, name);

	if (!ret)
	{
		//发送文件
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

	//读取http请求
	//1.读取请求行
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

		//判断方法是否合法
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

			//定位服务器本地html文件

			//定位url中的?
			char* pos = strchr(url, '?');
			if (pos)
			{
				*pos = '\0';
				if (debug) printf("real url %s\n", url);
			}


			sprintf(path, "./html_docs%s", url);
			if (debug) printf("path %s\n", path);

			//执行http响应

			if (stat(path, &st) == -1) // 路径不存在或出错
			{
				fprintf(stderr, "stat %s failed. reason: %s\n", path, strerror(errno));
				not_found(client_sock); //404页面返回
			}
			else   //路径存在
			{
				char* name = url;
				//判断是否是目录
				if (S_ISDIR(st.st_mode))
				{
					strcat(path, "index.html");
					name = (char*)"index.html";
					if (debug) printf("finally path: %s\n", path);
				}
				do_http_response(client_sock, path, name); //返回请求所需
			}
		}
		else
		{
			//其他请求
			if (debug) fprintf(stderr, "warning! other requst [%s]\n", method);//错误记录

			//只读取http请求头，不做任何处理
			do {
				len = get_line(client_sock, buf, sizeof(buf));
				printf("read line: %s\n", buf);
			} while (len > 0);

			unimplemented(client_sock);
		}
	}
	else // 出错处理
	{
		bad_request(client_sock);
	}
	close(client_sock);
	if (pclient_sock) free(pclient_sock); //释放动态分配的内存

	return NULL;
}

