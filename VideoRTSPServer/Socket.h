#pragma once
#include <WinSock2.h>
#include <mutex>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

/*
封装一个Socket，当sock引用减为0时自动closesocket
*/
class Socket
{
public:
	Socket(bool bIsTcp = true) {
		if (bIsTcp) {
			m_sock = socket(PF_INET, SOCK_STREAM, 0);
		}
		else {
			m_sock = socket(PF_INET, SOCK_DGRAM, 0);
		}
		m_pCount = new int(1);
		m_pMutex = new std::mutex;
	}
	Socket(const Socket& s) {
		m_sock = s.m_sock;
		m_pCount = s.m_pCount;
		m_pMutex = s.m_pMutex;
		AddCount();
	}
	~Socket() {
		Release();
	}
	Socket& operator=(const Socket& s){
		if (this != &s) {
			Release();
			m_pCount = s.m_pCount;
			m_pMutex = s.m_pMutex;
			m_sock = s.m_sock;
			AddCount();
		}
		return *this;
	}
	operator SOCKET() {
		return m_sock;
	}
	void Release() {
		bool deleteflag = false;
		m_pMutex->lock();
		if (--(*m_pCount) == 0) {
			delete m_pCount;
			closesocket(m_sock);
			std::cout << "close";
			deleteflag = true;
		}
		m_pMutex->unlock();
		if (deleteflag) {
			delete m_pMutex;
		}
	}
private:
	void AddCount() {
		m_pMutex->lock();
		++(*m_pCount);
		m_pMutex->unlock();
	}
private:
	SOCKET m_sock;
	int* m_pCount;			//计数器
	std::mutex* m_pMutex;	//锁
};

class SocketIniter
{
public:
	SocketIniter() {
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	~SocketIniter() {
		WSACleanup();
	}
};

