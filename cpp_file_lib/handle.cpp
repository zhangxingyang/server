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

	//����ͷ
	ret = headers(client_sock, name);

	if (!ret) {
		//�����ļ�
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

	//����ͷ
	ret = error_headers(client_sock, kind);

	if (!ret) {
		//�����ļ�
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

	//��ȡhttp����
	//1.��ȡ������
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

		//�жϷ����Ƿ�Ϸ�
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

			//��λ����������html�ļ�

			//��λurl�е�?
			char* pos = strchr((char*)url.c_str(), '?');
			if (pos) {
				*pos = '\0';
			}

			sprintf((char*)path.c_str(), "./html_docs%s", url.c_str());
			if (debug) printf("path: %s\n", path.c_str());

			//ִ��http��Ӧ
			printf("real path %s\n", path.c_str());
			if (stat(path.c_str(), &st) == -1) // ·�������ڻ����
			{
				fprintf(stderr, "stat %s failed. reason: %s\n", path.c_str(), strerror(errno));
				not_found(client_sock); //404ҳ�淵��
			}
			else   //·������
			{
				string name = url;
				//�ж��Ƿ���Ŀ¼
				if (S_ISDIR(st.st_mode))
				{
					//path.append("index.html");
					path += "index.html";
					name = "index.html";
					if (debug) printf("finally path: %s\n", path.c_str());
				}
				do_http_response(client_sock, path, name); //������������
			}
		}
		else
		{
			//��������
			fprintf(stderr, "warning! other requst [%s]\n", method.c_str());//�����¼

			//ֻ��ȡhttp����ͷ�������κδ���
			do {
				len = read_line(client_sock, buf, sizeof(buf));
				//printf("read line: %s\n", buf);
			} while (len > 0);

			unimplemented(client_sock);
		}
	}
	else {  // ������
		bad_request(client_sock);
	}

	close(client_sock);
	if (pclient_sock) free(pclient_sock); //�ͷŶ�̬������ڴ�
	
	return NULL;
}

