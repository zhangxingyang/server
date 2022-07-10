#include "../header_file_lib/send.h"

//�ɹ���ȡ����0�� ���ɹ�Ϊ-1
int headers(int client_sock, string name)
{
	//��������ͷ
	
	string buf;
	
	buf.append("HTTP/1.0 200 OK\r\n");
	buf.append("Server: Star Server\r\n");
	buf.append("Content-Type: " + get_mime_type(name) + "\r\n");
	buf.append("Connection: keep-alive\r\n");
	buf.append("\r\n");

	if (debug) fprintf(stdout, "header: %s\n", buf.c_str());

	if (send(client_sock, buf.c_str(), buf.size(), 0) < 0)
	{
		fprintf(stderr, "send failed. data: %s reason: %s\n", buf.c_str(), strerror(errno));
		return -1;
	}
	return 0;
}

int error_headers(int client_sock, string kind)
{
	//��������ͷ
	string buf;

	buf.append("HTTP/1.0 " + kind + "\r\n");
	buf.append("Server: Star Server\r\n");
	buf.append("Content-Type: text/html; charset=utf-8\r\n");
	buf.append("\r\n");

	if (debug) fprintf(stdout, "header: %s\n", buf.c_str());

	if (send(client_sock, buf.c_str(), buf.size(), 0) < 0)
	{
		fprintf(stderr, "send error information failed. data: %s reason: %s\n", buf.c_str(), strerror(errno));
		return -1;
	}
	return 0;
}


void cat(int client_sock, string path)
{
	ifstream ifs;
	ifs.open(path, ios::in);
	string buf;
	while (getline(ifs, buf))
	{
		int len = write(client_sock, buf.c_str(), buf.size());

		//	//���ͳ�������
		if (len < 0)
		{
			fprintf(stderr, "send body error. reason: %s\n", strerror(errno));
			break;
		}
	}
}

