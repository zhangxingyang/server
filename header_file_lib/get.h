#pragma once

#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

int read_line(int sock, char* buf, int size); //��ȡ����
string get_mime_type(string name); // �õ�����