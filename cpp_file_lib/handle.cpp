#include "../header_file_lib/handle.h"

void do_http_response(int client_sock, string path, string name)
{
	int ret = 0;
	FILE* resource = NULL;

	resource = fopen(path.c_str(), "r");

	int fileid = 0;
	struct stat st;

	fileid = fileno(resource);
	if (fstat(fileid, &st) == -1)
	{
		inner_error(client_sock);
		return;
	}

	//发送头
	ret = headers(client_sock, name);

	if (!ret) {
		//发送文件
		cat(client_sock, path);
	}

	fclose(resource);
}

void do_error_response(int client_sock, string path, string kind)
{
	int ret = 0;
	FILE* resource = NULL;

	path.insert(0, "./html_docs/error/");
	resource = fopen(path.c_str(), "r");

	int fileid = 0;
	struct stat st;

	fileid = fileno(resource);

	//发送头
	ret = error_headers(client_sock, kind);

	if (!ret) {
		//发送文件
		cat(client_sock, path);
	}

	fclose(resource);
}

void* do_http_request(void* pclient_sock)
{
	int len = 0;
	char buf[1024];
	string method;
	string url;
	string path;
	int client_sock = *(int*)pclient_sock;

	struct stat st;

	//读取http请求
	//1.读取请求行
	len = read_line(client_sock, buf, sizeof(buf));

	if (len > 0)
	{
		int i = 0, j = 0;
		while (!isspace(buf[j]) && (i < 255))
		{
			method += buf[j];
			i++;
			j++;
		}
		//method[i] = '\0';
		method.append("\0");

		//判断方法是否合法
		if (strncasecmp(method.c_str(), "GET", i) == 0)
		{
			printf("requst = %s\n", method.c_str());
			while (isspace(buf[++j]));
			i = 0;
			while (!isspace(buf[j]) && (i < 256))
			{
				url += buf[j];
				i++;
				j++;
			}
			if (debug) printf("url : %s\n", url.c_str());
			//index.html => html_path
			path.append("./html_docs");
			path += url;

			printf("path: %s\n", path.c_str());
			do {
				len = read_line(client_sock, buf, sizeof(buf));
				printf("read line: %s\n", buf);
			} while (len > 0);

			//定位服务器本地html文件

			//定位url中的?
			char* pos = strchr((char*)url.c_str(), '?');
			if (pos) {
				*pos = '\0';
			}

			sprintf((char*)path.c_str(), "./html_docs%s", url.c_str());
			if (debug) printf("path: %s\n", path.c_str());

			//执行http响应
			printf("real path %s\n", path.c_str());
			if (stat(path.c_str(), &st) == -1) // 路径不存在或出错
			{
				fprintf(stderr, "stat %s failed. reason: %s\n", path.c_str(), strerror(errno));
				not_found(client_sock); //404页面返回
			}
			else   //路径存在
			{
				string name = url;
				//判断是否是目录
				if (S_ISDIR(st.st_mode))
				{
					//path.append("index.html");
					path += "index.html";
					name = "index.html";
					if (debug) printf("finally path: %s\n", path.c_str());
				}
				do_http_response(client_sock, path, name); //返回请求所需
			}
		}
		else
		{
			//其他请求
			fprintf(stderr, "warning! other requst [%s]\n", method.c_str());//错误记录

			//只读取http请求头，不做任何处理
			do {
				len = read_line(client_sock, buf, sizeof(buf));
				//printf("read line: %s\n", buf);
			} while (len > 0);

			unimplemented(client_sock);
		}
	}
	else {  // 出错处理
		bad_request(client_sock);
	}

	close(client_sock);
	if (pclient_sock) free(pclient_sock); //释放动态分配的内存
	
	return NULL;
}

