#include "../header_file_lib/get.h"

int read_line(int sock, char* buf, int size) //读取请求
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
		else if (len == -1) {  //读取出错
			perror("read failed.");
			break;
		}
		else {  //客户端socket 关闭 read返回0
			fprintf(stderr, "client close.\n");
			break;
		}
	}
	return count;
}

string get_mime_type(string name) // 得到类型
{
	char* dot;

	dot = strrchr((char*)name.c_str(), '.'); //自右向左查找‘.’字符;如不存在返回NULL
	/*
	 *charset=iso-8859-1 西欧的编码，说明网站采用的编码是英文；
	 *charset=gb2312  说明网站采用的编码是简体中文；
	 *charset=utf-8   代表世界通用的语言编码；
	 *      可以用到中文、韩文、日文等世界上较多语言编码上
	 *charset=euc-kr  说明网站采用的编码是韩文；
	 *charset=big5   说明网站采用的编码是繁体中文；
	 *
	 *以下是依据传递进来的文件名，使用后缀判断是何种文件类型
	 *将对应的文件类型按照http定义的关键字发送回去
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