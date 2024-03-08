#pragma once
#include "Socket.h"
#include <Windows.h>
#include "CBuffer.h"

#include "Base.h"

struct RTPHeader
{
public:
	//RTPheader����
	//ע�ⵥ���ֽ�bitд��˳��
	unsigned csrccount : 4;				//��Լ��ԴCSRC����
	unsigned extension : 1;				//��չλ��1��ʾ��ͷ���滹��һ����չͷ��
	unsigned padding : 1;				//���λ��1��ʾ��β���и����ֽ�
	unsigned version : 2;				//�汾��

	unsigned pytype : 7;				//�������ͣ�һ����������Ƶ����ĸ�ʽ
	unsigned mark : 1;					//���λ���������ĵ���

	unsigned serial : 16;				//���к�

	unsigned timestamp : 32;			//ʱ�������¼���ݺ��ز��ֲ���ʱ��
	unsigned ssrc : 32;					//ͬ����ԴSSRC�����RTP����Դ��һ��RTP�Ự�в���������SSRC
	unsigned csrc[15];					//��Լ��ԴCSRC
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

//һ��RTP���ĳ�������1400�ֽڣ���Ϊʹ��UDP����
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

