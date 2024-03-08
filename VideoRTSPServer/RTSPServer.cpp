#include "RTSPServer.h"
#include <rpc.h>
#pragma comment(lib, "rpcrt4.lib")

RTSPServer::RTSPServer()
{
    SocketIniter::Init();
    m_socket = Socket(true);
    m_status = 0;
    m_threadPool.Init();
}

int RTSPServer::Init(const std::string& strIP, short port)
{
    //服务端套接字初始化
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.s_addr = inet_addr(strIP.c_str());
    int ret = m_socket.Bind(&m_addr);
    ret = m_socket.Listen(5);
    return 0;
}

int RTSPServer::Invoke()
{
    //开始主线程
    m_threadMain = std::thread(&RTSPServer::ThreadMain,this);
    m_threadMainExit = false;
    return 0;
}

void RTSPServer::Stop()
{
    m_threadMainExit = true;
    m_socket.Release();
    SocketIniter::Clear();
    m_threadPool.Shutdown();
    if (m_threadMain.joinable()) {
        m_threadMain.join();
    }
}

RTSPServer::~RTSPServer()
{
    Stop();
}

void RTSPServer::ThreadMain()
{
    while (!m_threadMainExit) {
        sockaddr_in client_addr;
        Socket client = m_socket.Accept(client_addr);
        if (client != INVALID_SOCKET) {
            //连接上一个客户端则创建一个会话，同时开启一个会话线程
            RTSPSession session(client);
            m_listSession.Push(session);
            m_threadPool.SubmitTask(Task(&RTSPServer::ThreadSession, (void*)this));
        }
    }
    std::cout << "main exit" << std::endl;
}

void RTSPServer::ThreadSession(void* arg)
{
    int ret = 0;
    RTSPServer* s = (RTSPServer*)arg;

    RTSPSession session;
    //取出一个会话进行信息交互
    if (s->m_listSession.Size() == 0) {
        return;
    }
    session = s->m_listSession.GetFront();
    if (session.m_client != INVALID_SOCKET) {
        ret = session.PickRequestAndReply(s, PlayCallBack);
    }
    session.SessionClose();
}

void RTSPServer::PlayCallBack(void* arg)
{
    PlayCallBackArg* a = (PlayCallBackArg*)arg;
    RTSPServer* server = a->server;
    RTSPSession* session = a->session;
    RTPFrame rtpframe;
    session->m_mediafile->Reset();
    CBuffer frameData = session->m_mediafile->ReadOneFrame();
    while (frameData.size() > 0) {
        //获取udp的addr
        sockaddr clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        getsockname(session->m_client, &clientAddr, &clientAddrLen);
        ((sockaddr_in*)(&clientAddr))->sin_port = htons(session->m_client_udp_port);
        //发送udp数据
        server->m_rtphelper.SendMediaFrame(rtpframe, frameData, clientAddr);
        frameData = session->m_mediafile->ReadOneFrame();
    }
}

RTSPSession::RTSPSession() 
{
    UUID uuid;
    UuidCreate(&uuid);
    m_id.resize(10);
    snprintf((char*)m_id.c_str(), m_id.size(), "%u%u", uuid.Data1, uuid.Data2);
    m_mediafile = new MediaFile();
    m_mediafile->Open(MediaPath, 96);
}

RTSPSession::RTSPSession(const Socket& client)
{
    //客户端套接字构造会话类
    m_client = client;
    UUID uuid;
    UuidCreate(&uuid);
    m_id.resize(10);
    snprintf((char*)m_id.c_str(), m_id.size(), "%u%u", uuid.Data1, uuid.Data2);
    m_mediafile = new MediaFile();
    m_mediafile->Open(MediaPath, 96);
}

RTSPSession::RTSPSession(const RTSPSession& ss)
{
    m_client = ss.m_client;
    m_id = ss.m_id;
    m_client_udp_port = ss.m_client_udp_port;
    m_mediafile = ss.m_mediafile;
}

int RTSPSession::PickRequestAndReply(RTSPServer* server, void (*playCallback)(void* arg))
{
    int ret = -1;
    do{
        CBuffer buf = Pick();                       //接受数据
        if (buf.size() == 0) {
            return -1;
        }
        RTSPRequest req = AnalyseRequest(buf);      //解析数据
        if (req.m_method < 0) {
            return -2;
        }
        RTSPReply rep = MakeReply(req);             //回复客户端的数据
        if (req.m_method == 2) {
            m_client_udp_port = (unsigned short)atoi(req.m_client_port[0].c_str());
        }
        if (req.m_method == 3) {
            //当指令值Play时开个线程发送视频流
            struct PlayCallBackArg* arg = new struct PlayCallBackArg;
            arg->server = server;
            arg->session = this;
            server->ThreadPool().SubmitTask(Task(&RTSPServer::PlayCallBack, (void*)arg));
        }
        ret = m_client.Send(rep.toBuffer());    //回复客户端
        if (req.m_method == 4) {
            //视频发送完成，关闭连接
            break;
        }
    } while (ret > 0);
    if (ret < 0) {
        return ret;
    }
    return 0;
}

CBuffer RTSPSession::PickOneLine(CBuffer& buf)
{
    CBuffer ret;
    if (buf.size() == 0) return ret;
    int i = 0;
    for (; i < buf.size(); i++) {
        ret += buf[i];
        if (buf[i] == '\n') {
            break;
        }
    }
    CBuffer temp(buf.c_str() + i + 1);
    buf = temp;

    return ret;
}

CBuffer RTSPSession::Pick()
{
    CBuffer result;         //一次完整数据
    CBuffer buf(1);         //缓冲区，大小为1
    int reclen = 1;
    while (reclen > 0) {
        buf.SetZero();
        reclen = m_client.Recv(buf);    //每次读取一个字节到result中
        if (reclen > 0) {
            result += buf;
            if (result.size() > 4) {
                //通过数据后四个字节判断是否读取完整，不完整继续读取下一个字节
                int val = *(int*)(result.c_str() + result.size() - 4);
                if (val == *(int*)"\r\n\r\n") {
                    break;
                }
            }
        }
    }
   //ATLTRACE("接受数据:\r\n%s\r\n", result.c_str());
    return result;
}

RTSPRequest RTSPSession::AnalyseRequest(const CBuffer& buffer)
{
    //RTSP交互协议内容      客户端->服务端
    RTSPRequest req;
    CBuffer buf = buffer;
    CBuffer line;
    line = PickOneLine(buf);
    CBuffer method(32), version(64), url(1024), session(64);
    if (sscanf(line.c_str(), "%s %s %s\r\n", (char *)method.c_str(), (char*)url.c_str(), (char*)version.c_str()) < 3) {
        ATLTRACE("error at:[%s]\r\n", line.c_str());
        return req;
    }
    line = PickOneLine(buf);
    if (sscanf(line.c_str(), "CSeq: %s\r\n", (char*)req.m_seq.c_str()) < 1) {
        ATLTRACE("error at:[%s]\r\n", line.c_str());
        return req;
    }
    req.SetMethod(method);
    req.SetUrl(url);
    line = PickOneLine(buf);
    switch (req.m_method)
    {
    case 0://OPTIONS
    case 1://DESCRIBE
        return req;
        break;
    case 2://SETUP
        line = PickOneLine(buf);
        int port[2];
        if (sscanf(line.c_str(), "Transport: RTP/AVP;unicast;client_port=%d-%d\r\n", &port[0], &port[1]) == 2) {
            req.SetClientPort(port);
            return req;
        }
        else {
            ATLTRACE("error at:[%s]\r\n", line.c_str());
        }
        break;
    case 3://PLAY
    case 4://TEARDOWN
        line = PickOneLine(buf);
        if (sscanf(line.c_str(), "Session: %s\r\n", (char*)session.c_str()) == 1) {
            req.SetSession(session);
            return req;
        }
        else {
            ATLTRACE("error at:[%s]\r\n", line.c_str());
        }
        break;
    default:
        break;
    }

    return req;
}

RTSPReply RTSPSession::MakeReply(const RTSPRequest& req)
{
    //根据请求信息，定义回复信息
    RTSPReply rep;
    rep.SetSequence(req.m_seq);
    if (req.m_session.size() > 0) {
        rep.SetSession(req.m_session);
    }
    else {
        rep.SetSession(m_id);
    }
    rep.SetMethod(req.m_method);
    rep.SetSequence(req.m_seq);
    switch (req.m_method)
    {
    case 0://OPTIONS
        rep.SetOptions("Public: OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN\r\n");
        break;
    case 1://DESCRIBE
    {
        //sdp协议的处理
        CBuffer sdp;
        sdp << "v=0\r\n";
        sdp << "o=- " << m_id.c_str() << " 1 IN IP4 127.0.0.1\r\n";
        sdp << "t=0 0\r\n" << "a=control:*\r\n" << "m=video 0 RTP/AVP 96\r\n";
        sdp << "a=framerate:24\r\n";
        sdp << "a=rtpmap:96 H264/90000\r\n" << "a=control:track0\r\n";
        rep.SetSdp(sdp);
    }
    break;
    case 2://SETUP
        rep.SetClientPort(req.m_client_port[0], req.m_client_port[1]);
        rep.SetServerPort("5454", "5455");
        break;
    case 3://PLAY
    case 4://TEARDOWN
        break;
    default:
        break;
    }

    return rep;
}

RTSPReply::RTSPReply() :m_method(-1) {
    m_client_port[0] = 0; m_client_port[1] = 0;
    m_server_port[0] = 0; m_server_port[1] = 0;
}

RTSPReply::RTSPReply(const RTSPReply& rep)
{
    m_client_port[0] = rep.m_client_port[0];
    m_client_port[1] = rep.m_client_port[1];
    m_server_port[0] = rep.m_server_port[0];
    m_server_port[1] = rep.m_server_port[1];
    m_method = rep.m_method;
    m_options = rep.m_options;
    m_sdp = rep.m_sdp;
    m_seq = rep.m_seq;
    m_session = rep.m_session;
}

CBuffer RTSPReply::toBuffer()
{
    //RTSP交互协议内容      服务端->客户端
    CBuffer ret;
    ret << "RTSP/1.0 200 OK\r\n" << "CSeq: " << m_seq << "\r\n";
    switch (m_method)
    {
    case 0://OPTIONS
        ret << "Public: OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN\r\n\r\n";
        break;
    case 1://DESCRIBE
        ret << "Content-Base: 127.0.0.1\r\n";
        ret << "Content-type: application/sdp\r\n";
        ret << "Content-length: " << std::to_string(m_sdp.size()) << "\r\n\r\n";
        ret << m_sdp;
        break;
    case 2://SETUP
        ret << "Transport: RTP/AVP;unicast;client_port=" << std::to_string(m_client_port[0]) << "-" << std::to_string(m_client_port[1]);
        ret << ";server_port=" << std::to_string(m_server_port[0]) << "-" << std::to_string(m_server_port[1]) << "\r\n";
        ret << "Session: " << m_session << "\r\n\r\n";
        break;
    case 3://PLAY
        ret << "Range: npt=0.000-\r\n";
        ret << "Session: " << m_session << "\r\n\r\n";
        break;
    case 4://TEARDOWN
        ret << "Session: " << m_session << "\r\n\r\n";
        break;
    }
    std::cout << ret << std::endl;
    return ret;
}

void RTSPReply::SetMethod(const int m)
{
    m_method = m;
}

void RTSPReply::SetOptions(const std::string& options)
{
    m_options = options;
}

void RTSPReply::SetSequence(const std::string& seq)
{
    m_seq = seq.c_str();
}

void RTSPReply::SetSdp(const CBuffer& sdp)
{
    m_sdp = sdp;
}

void RTSPReply::SetClientPort(const std::string& port0, const std::string& port1)
{
    m_client_port[0] = atoi(port0.c_str());
    m_client_port[1] = atoi(port1.c_str());
}

void RTSPReply::SetServerPort(const std::string& port0, const std::string& port1)
{
    m_server_port[0] = atoi(port0.c_str());
    m_server_port[1] = atoi(port1.c_str());
}

void RTSPReply::SetSession(const std::string& session)
{
    m_session = session;
}

RTSPRequest::RTSPRequest(const RTSPRequest& req)
{
    m_client_port[0] = req.m_client_port[0];
    m_client_port[1] = req.m_client_port[1];
    m_method = req.m_method;
    m_seq = req.m_seq;
    m_session = req.m_session;
    m_url = req.m_url;
}

void RTSPRequest::SetMethod(std::string& m)
{
    std::string str;
    for (auto& c : m) {
        if (c == '\0') {
            break;
        }
        str += c;
    }
    if (str=="OPTIONS")
        m_method = 0;
    else if (str=="DESCRIBE")
        m_method = 1;
    else if (str=="SETUP")
        m_method = 2;
    else if (str=="PLAY")
        m_method = 3;
    else if (str=="TEARDOWN")
        m_method = 4;
}

void RTSPRequest::SetUrl(std::string& m)
{
    for (auto& c : m) {
        if (c == '\0') {
            break;
        }
        m_url += c;
    }
}

void RTSPRequest::SetClientPort(const int port[])
{
    m_client_port[0] = std::to_string(port[0]);
    m_client_port[1] = std::to_string(port[1]);
}

void RTSPRequest::SetSession(std::string& m)
{
    for (auto& c : m) {
        if (c == '\0') {
            break;
        }
        m_session += c;
    }
}

RTSPRequest& RTSPRequest::operator=(const RTSPRequest& req)
{
    if (this != &req) {
        m_client_port[0] = req.m_client_port[0];
        m_client_port[1] = req.m_client_port[1];
        m_method = req.m_method;
        m_seq = req.m_seq;
        m_session = req.m_session;
        m_url = req.m_url;
    }
    return *this;
}

RTSPReply& RTSPReply::operator=(const RTSPReply& rep)
{
    if (this != &rep) {
        m_client_port[0] = rep.m_client_port[0];
        m_client_port[1] = rep.m_client_port[1];
        m_server_port[0] = rep.m_server_port[0];
        m_server_port[1] = rep.m_server_port[1];
        m_method = rep.m_method;
        m_options = rep.m_options;
        m_sdp = rep.m_sdp;
        m_seq = rep.m_seq;
        m_session = rep.m_session;
    }
    return *this;
}
