#pragma once
#include <WinSock2.h>
#include <mutex>
#include <iostream>
#include "Base.h"
#include "CBuffer.h"
#pragma comment(lib, "ws2_32.lib")

/*
封装一个Socket，当sock引用减为0时自动closesocket
*/
class Socket
{
public:
	Socket() {
		m_sock = INVALID_SOCKET;
		m_pCount = nullptr;
		m_pMutex = nullptr;
	}
	Socket(bool bIsTcp) {
		if (bIsTcp) {
			m_sock = socket(AF_INET, SOCK_STREAM, 0);
		}
		else {
			m_sock = socket(AF_INET, SOCK_DGRAM, 0);
		}
		ATLTRACE("create socket: %d\r\n", m_sock);
		m_pCount = new int(1);
		m_pMutex = new std::mutex;
	}
	Socket(const Socket& s) {
		m_sock = s.m_sock;
		m_pCount = s.m_pCount;
		m_pMutex = s.m_pMutex;
		AddCount();
	}
	Socket(const SOCKET& s) {
		m_sock = s;
		ATLTRACE("create socket: %d\r\n", m_sock);
		m_pCount = new int(1);
		m_pMutex = new std::mutex;
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
		if (m_sock == INVALID_SOCKET) {
			return;
		}
		bool deleteflag = false;
		m_pMutex->lock();
		if (--(*m_pCount) == 0) {
			delete m_pCount;
			ATLTRACE("close socket: %d\r\n", m_sock);
			closesocket(m_sock);
			m_sock = INVALID_SOCKET;
			deleteflag = true;
		}
		else {
			//ATLTRACE("reduce socket: %d, now count=%d\r\n", m_sock, *m_pCount);
		}
		m_pMutex->unlock();
		if (deleteflag) {
			delete m_pMutex;
		}
	}
	int Bind(const sockaddr_in* addr) {
		if (m_sock == INVALID_SOCKET) {
			return -1;
		}
		int ret = bind(m_sock, (sockaddr*)addr, sizeof(sockaddr));
		if (ret < 0) {
			std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
		}
		return ret;
	}
	int Listen(int backlog = 5) {
		if (m_sock == INVALID_SOCKET) {
			std::cerr << "Listen failed with error" << std::endl;
			return -1;
		}
		return listen(m_sock, backlog);
	}
	int Connect(const sockaddr_in& addr) {
		int ret = connect(m_sock, (sockaddr*)&addr, sizeof(sockaddr_in));
		if(ret<0){
			std::cerr << "Connect failed with error" << std::endl;
		}
		return ret;
	}
	Socket Accept(sockaddr_in& addr) {
		int len = sizeof(addr);
		Socket ret = Socket(accept(m_sock, (sockaddr*)&addr, &len));
		if (ret == INVALID_SOCKET) {
			std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
		}
		return ret;
	}
	int Recv(CBuffer& buffer) {
		return recv(m_sock, (char *)buffer.c_str(), buffer.size(), 0);
	}
	int Send(const CBuffer& buffer) {
		int index = 0;
		char* pdata = (char*)buffer.c_str();
		while (index < buffer.size()) {
			int ret = send(m_sock, pdata + index, buffer.size() - index, 0);
			if (ret < 0) return -1;
			if (ret == 0) break;
			index += ret;
		}

		return index;
	}

private:
	void AddCount() {
		m_pMutex->lock();
		++(*m_pCount);
		//ATLTRACE("add socket: %d, now count=%d\r\n", m_sock, *m_pCount);
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
	SocketIniter() {}
	~SocketIniter() {}
	static void Init() {
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	static void Clear() {
		WSACleanup();
	}
};

