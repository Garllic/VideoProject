#pragma once
#include <WinSock2.h>
class Socket
{
public:
	Socket(bool bIsTcp = true) {
		m_sock = INVALID_SOCKET;
		if (bIsTcp) {
			m_sock = socket(PF_INET, SOCK_STREAM, 0);
		}
		else {
			m_sock = socket(PF_INET, SOCK_DGRAM, 0);
		}
	}
	void Close() {
		closesocket(m_sock);
	}
	~Socket() {}
private:

	SOCKET m_sock;
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

