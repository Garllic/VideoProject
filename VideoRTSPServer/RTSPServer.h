#pragma once
#include "Socket.h"
#include <string>
#include <map>
#include "CThreadPool.h"
#include <atltrace.h> 
#include "RTPHelper.h"
#include "MediaFile.h"

class RTSPRequest 
{
public:
	RTSPRequest():m_method(-1) {}
	RTSPRequest(const RTSPRequest& req);
	void SetMethod(std::string& m);
	void SetUrl(std::string& m);
	void SetClientPort(const int port[]);
	void SetSession(std::string& m);
	~RTSPRequest() {}
	RTSPRequest& operator=(const RTSPRequest& req);
public:
	int m_method;//0 OPTIONS 1 DESCRIBE 2 SETUP 3 PLAY 4 TEARDOWN
	std::string m_url;
	std::string m_session;
	std::string m_seq;
	std::string m_client_port[2];
};

class RTSPReply 
{
public:
	RTSPReply();
	RTSPReply(const RTSPReply& rep);
	~RTSPReply() {}
	CBuffer toBuffer();
	void SetMethod(const int m);
	void SetOptions(const std::string& options);
	void SetSequence(const std::string& seq);
	void SetSdp(const CBuffer& sdp);
	void SetClientPort(const std::string& port0, const std::string& port1);
	void SetServerPort(const std::string& port0, const std::string& port1);
	void SetSession(const std::string& session);
	RTSPReply& operator=(const RTSPReply& rep);
private:
	int m_method;//0 OPTIONS 1 DESCRIBE 2 SETUP 3 PLAY 4 TEARDOWN
	std::string m_sdp;
	std::string m_options;
	std::string m_seq;
	std::string m_session;
	int m_client_port[2];
	int m_server_port[2];
};

class RTSPServer;

class RTSPSession 
{
public:
	std::string m_id;
	Socket m_client;
	unsigned short m_client_udp_port;
	MediaFile* m_mediafile;
public:
	RTSPSession();
	RTSPSession(const Socket& client);
	RTSPSession(const RTSPSession& ss);
	int PickRequestAndReply(RTSPServer* server, void (*playCallback)(void* arg));		//���ܿͻ������ݣ��������ظ�
	void SessionClose() {
		m_mediafile->Close();
		delete m_mediafile;
	}
	~RTSPSession() {}
private:
	CBuffer PickOneLine(CBuffer& buf);
	CBuffer Pick();										//���ܿͻ�������
	RTSPRequest AnalyseRequest(const CBuffer& buffer);	//�������ܵ�������
	RTSPReply MakeReply(const RTSPRequest& req);		//�õ��ظ��ͻ��˵�����
};

struct PlayCallBackArg
{
	RTSPServer* server;
	RTSPSession* session;
};

class RTSPServer
{
public:
	RTSPServer();
	int Init(const std::string& strIP = "0.0.0.0", short port= 554);
	int Invoke();
	void Stop();
	CThreadPool& ThreadPool() { return m_threadPool; }
	static void PlayCallBack(void* arg);		//playָ��ص�����
	~RTSPServer();
protected:
	void ThreadMain();							//���̺߳��������ܿͻ��˵���������
	static void ThreadSession(void* arg);		//�Ự�̺߳����������������ϵĿͻ��˵���Ϣ�Ự
private:
	Socket m_socket;	//�������׽���
	int m_status;		//0 δ��ʼ��  1 ��ʼ�����  2 ��������  3 �ر�
	std::thread m_threadMain;
	bool m_threadMainExit;
	CThreadPool m_threadPool;
	sockaddr_in m_addr;//��������ַ
	ShareQueue<RTSPSession> m_listSession;
	RTPHelper m_rtphelper;
};

