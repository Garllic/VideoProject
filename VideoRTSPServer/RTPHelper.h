#pragma once
#include "Socket.h"
#include <Windows.h>
#include "CBuffer.h"

#include "Base.h"

struct RTPHeader
{
public:
	//RTPheader定义
	//注意单个字节bit写入顺序
	unsigned csrccount : 4;				//特约信源CSRC个数
	unsigned extension : 1;				//扩展位，1表示包头后面还有一个扩展头部
	unsigned padding : 1;				//填充位，1表示包尾部有附加字节
	unsigned version : 2;				//版本号

	unsigned pytype : 7;				//荷载类型，一般依据音视频本身的格式
	unsigned mark : 1;					//标记位，给配置文档用

	unsigned serial : 16;				//序列号

	unsigned timestamp : 32;			//时间戳，记录数据荷载部分采样时刻
	unsigned ssrc : 32;					//同步信源SSRC，标记RTP的来源，一个RTP会话中不能有两个SSRC
	unsigned csrc[15];					//特约信源CSRC
public:
	RTPHeader();
	operator CBuffer();
};

struct RTPFrame
{
public:
	RTPHeader m_head;
	CBuffer m_pyload;
public:
	operator CBuffer();
};

//一个RTP包的长度限制1400字节，因为使用UDP发送
class RTPHelper
{
private:
	unsigned m_timeStamp;
	Socket m_udp;
public:
	RTPHelper();
	int SendMediaFrame(RTPFrame& rtpframe,CBuffer& frame, const sockaddr& addr);
private:
	int SendFrame(CBuffer rtpframe, const sockaddr addr);
};

