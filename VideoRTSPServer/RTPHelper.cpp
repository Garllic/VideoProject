#include "RTPHelper.h"

RTPHelper::RTPHelper():m_timeStamp(0)
{
    SocketIniter::Init();
    m_udp = Socket(false);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ADDR_ANY;
    addr.sin_port = htons(55000);
    m_udp.Bind(&addr);
}

int RTPHelper::SendMediaFrame(RTPFrame& rtpframe, CBuffer& frame, const sockaddr& addr)
{
    size_t frameSize = frame.size() - 4;            //h264���ݶ�֡��С(�ų��ָ���)
    BYTE* frameData = (BYTE*)(frame.c_str() + 4);   //h264����֡(�ų��˷ָ���)

    if (frameSize > MAX_DATA) {  //��Ƭ
        int dataCount = (frameSize % MAX_DATA == 0 ? frameSize / MAX_DATA : ceil((float)frameSize / MAX_DATA));
        rtpframe.m_pyload.resize(MAX_DATA + 2);

        for (int i = 0; i < dataCount; i++) {
            if (i == dataCount - 1) {
                rtpframe.m_pyload.resize(frameSize % MAX_DATA + 2);
            }
            rtpframe.m_pyload[0] = 0b01111100;              //��һ���ֽ�     ״̬��1  ��Ҫ����2  ���ͣ�5
            rtpframe.m_pyload[1] = frameData[0] & 0x1F;     //�ڶ����ֽ�     ��ʼ��־��1    ������־��1  ����λ��1   ��һ֡����λ��5
            if (i == 0) {
                rtpframe.m_pyload[1] = rtpframe.m_pyload[1] | 0x80;
            }
            else if (i == dataCount - 1) {
                rtpframe.m_pyload[1] = rtpframe.m_pyload[1] | 0x40;
            }
            memcpy((void*)(2 + rtpframe.m_pyload.c_str()), (void*)(frameData + MAX_DATA * i + 1), rtpframe.m_pyload.size() - 2);
            rtpframe.m_head.timestamp = m_timeStamp;
            if (SendFrame(rtpframe, addr) <= 0) {
                ATLTRACE("sendframe error!\r\n");
            }
            rtpframe.m_head.serial++;
        }
    }
    else {  //����Ƭ
        rtpframe.m_pyload.resize(frameSize);
        memcpy((void*)rtpframe.m_pyload.c_str(), (void*)frameData, frameSize);
        rtpframe.m_head.timestamp = m_timeStamp;
        if (SendFrame(rtpframe, addr) <= 0) {
            ATLTRACE("sendframe error!\r\n");
        }
        rtpframe.m_head.serial++;
    }
    m_timeStamp += 90000 / 24;
    Sleep(1000 / 24);       //ÿ��30֡

    return 0;
}

int RTPHelper::SendFrame(CBuffer rtpframe, const sockaddr addr)
{
    int ret = sendto(m_udp, rtpframe.c_str(), rtpframe.size(), 0, &addr, sizeof(addr));
    printf("ret=%d, rtpframe_size=%d, ip=%s, port=%d\r\n", ret, rtpframe.size(), inet_ntoa(((sockaddr_in*)(&addr))->sin_addr), ntohs(((sockaddr_in*)(&addr))->sin_port));

    return ret;
}

RTPHeader::RTPHeader()
{
    csrccount = 0;
    extension = 0;
    padding = 0;
    version = 2;
    pytype = 96;
    mark = 0;
    serial = 0;
    timestamp = 0;
    ssrc = 0x98765432;
    memset((void*)csrc, 0, sizeof(csrc));
}

RTPHeader::operator CBuffer()
{
    size_t s = 12 + csrccount * 4;
    CBuffer ret(s);
    RTPHeader header = *this;
    header.serial = htons(header.serial);
    header.timestamp = htonl(header.timestamp);
    header.ssrc = htonl(header.ssrc);
    memcpy((void*)ret.c_str(), &header, s);

    return ret;
}

RTPFrame::operator CBuffer()
{
    CBuffer ret;
    ret += m_head;
    ret += m_pyload;

    return ret;
}
