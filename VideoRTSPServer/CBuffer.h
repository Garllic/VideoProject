#pragma once
#include <string>
/*
��װһ��Buffer,���ڽ��ܷ�������
*/
class CBuffer :public std::string
{
public:
	//char*:�ַ���
	CBuffer(const char* str) {
		resize(strlen(str));
		memcpy((void*)c_str(), str, size());
	}
	//s:cbuffer�Ĵ�С
	CBuffer(size_t s = 0) {
		if (s > 0) {
			resize(s);
			memset((void*)c_str(), 0, s);
		}
	}
	CBuffer(void* buffer, size_t s) {
		resize(s);
		memcpy((void*)c_str(), buffer, s);
	}
	~CBuffer() {}
	void Update(void* buffer, size_t s) {
		resize(s);
		memcpy((void*)c_str(), buffer, s);
	}
	//��buffer������0�����ı䳤��
	void SetZero() {
		if (size() > 0) {
			memset((void*)c_str(), 0, size());
		}
	}
	CBuffer& operator<<(const char* str) {
		if (str == this->c_str()) {
			CBuffer t = str;
			*this += t;
		}
		else {
			*this += CBuffer(str);
		}

		return *this;
	}
	CBuffer& operator<<(const std::string& str) {
		if (str == this->c_str()) {
			CBuffer t = str.c_str();
			*this += t;
		}
		else {
			*this += str;
		}

		return *this;
	}
};

