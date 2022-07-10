#include "../header_file_lib/get.h"

int read_line(int sock, char* buf, int size) //��ȡ����
{
	int count = 0;
	char ch = '\0';
	int len = 0;

	while ((count < size - 1) && ch != '\n')
	{
		len = read(sock, &ch, 1);
		sigset_t set;

		if (len == 1)
		{
			if (ch == '\r') {
				continue;
			}
			else if (ch == '\n') {
				buf[count] = '\0';
				break;
			}
			buf[count] = ch;
			count++;
		}
		else if (len == -1) {  //��ȡ����
			perror("read failed.");
			break;
		}
		else {  //�ͻ���socket �ر� read����0
			fprintf(stderr, "client close.\n");
			break;
		}
	}
	return count;
}

string get_mime_type(string name) // �õ�����
{
	char* dot;

	dot = strrchr((char*)name.c_str(), '.'); //����������ҡ�.���ַ�;�粻���ڷ���NULL
	/*
	 *charset=iso-8859-1 ��ŷ�ı��룬˵����վ���õı�����Ӣ�ģ�
	 *charset=gb2312  ˵����վ���õı����Ǽ������ģ�
	 *charset=utf-8   ��������ͨ�õ����Ա��룻
	 *      �����õ����ġ����ġ����ĵ������Ͻ϶����Ա�����
	 *charset=euc-kr  ˵����վ���õı����Ǻ��ģ�
	 *charset=big5   ˵����վ���õı����Ƿ������ģ�
	 *
	 *���������ݴ��ݽ������ļ�����ʹ�ú�׺�ж��Ǻ����ļ�����
	 *����Ӧ���ļ����Ͱ���http����Ĺؼ��ַ��ͻ�ȥ
	 */
	if (dot == (char*)0)
		return "text/plain; charset=utf-8";
	if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
		return "text/html; charset=utf-8";
	if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
		return "image/jpeg";
	if (strcmp(dot, ".gif") == 0)
		return "image/gif";
	if (strcmp(dot, ".png") == 0)
		return "image/png";
	if (strcmp(dot, ".ico") == 0)
		return "image/x-icon";
	if (strcmp(dot, ".css") == 0)
		return "text/css";
	if (strcmp(dot, ".au") == 0)
		return "audio/basic";
	if (strcmp(dot, ".wav") == 0)
		return "audio/wav";
	if (strcmp(dot, ".avi") == 0)
		return "video/x-msvideo";
	if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
		return "video/quicktime";
	if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
		return "video/mpeg";
	if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
		return "model/vrml";
	if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
		return "audio/midi";
	if (strcmp(dot, ".mp3") == 0)
		return "audio/mpeg";
	if (strcmp(dot, ".ogg") == 0)
		return "application/ogg";
	if (strcmp(dot, ".pac") == 0)
		return "application/x-ns-proxy-autoconfig";
	return "text/plain; charset=utf-8";
}