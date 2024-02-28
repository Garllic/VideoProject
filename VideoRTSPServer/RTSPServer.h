#pragma once
#include "Socket.h"
#include <string>
#include <map>
#include "CThreadPool.h"

class RTSPRequest 
{
public:
	RTSPRequest();
	~RTSPRequest();
private:
	int m_method;//0 OPTIONS 1 DESCRIBE 2 SETUP 3 PLAY 4 TEARDOWN
};

class RTSPReply 
{
public:
	RTSPReply();
	~RTSPReply();
private:
	int m_method;//0 OPTIONS 1 DESCRIBE 2 SETUP 3 PLAY 4 TEARDOWN
};

class RTSPSession 
{
public:
	RTSPSession();
	~RTSPSession();
};

class RTSPServer
{
public:
	RTSPServer();
	int Init(const std::string& strIP = "0.0.0.0", short port= 554);
	int Invoke();
	void Stop();
	~RTSPServer();
protected:
	int ThreadMain();
	RTSPRequest AnalyseRequest(const std::string& data);
	RTSPReply MakeReply(const RTSPRequest& requst);
private:
	static SocketIniter m_initer;
	Socket m_socket;
	int m_status;//0 未初始化  1 初始化完成  2 正在运行  3 关闭
	CThreadPool m_threadPool;
	std::map<std::string, RTSPSession> m_mapSession;
};

