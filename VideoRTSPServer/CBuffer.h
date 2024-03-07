#pragma once
#include <string>
/*
封装一个Buffer,用于接受发送数据
*/
class CBuffer :public std::string
{
public:
	//char*:字符串
	CBuffer(const char* str) {
		resize(strlen(str));
		memcpy((void*)c_str(), str, size());
	}
	//s:cbuffer的大小
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
	//将buffer数据置0，不改变长度
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

