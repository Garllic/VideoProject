#ifndef EVLC_H
#define EVLC_H

#include <vlc/vlc.h>
#include <string>
#include <QWidget>
#include <windows.h>

//设计数据结构保存MediaInfo
typedef struct {
    int nWidth;     //播放窗口宽
    int nHeight;    //播放窗口高
    long long tm;   //视频时长
}VSize;

//对vlc做一些简单的封装
class EVlc
{
public:
    bool reset;
protected:
    libvlc_instance_t* m_instance;
    libvlc_media_t* m_media;
    libvlc_media_player_t* m_player;
public:
    //初始化m_media，m_player
    int SetMedia(const std::string& strUrl);
    //视频播放
    int Play();
    //设置播放的窗口
    int SetWidget(QWidget* widget);
    //停止视频播放
    int Pause();
    //暂停视频播放
    int Stop();
    //获取当前播放进度
    float GetPosition();
    //设置当前播放进度
    int SetPosition(float pos);
    //获取音量
    int GetVolume();
    //设置音量
    int SetVolume(int volume);
    //获取播放信息
    VSize GetMediaInfo();

    EVlc& operator=(const EVlc& s);
    EVlc();
    EVlc(const EVlc& s);
    ~EVlc();
};


#endif // EVLC_H
