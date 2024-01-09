#ifndef VIDEOCLIENTCONTROLLER_H
#define VIDEOCLIENTCONTROLLER_H

#include "evlc.h"
#include "mainwindow.h"

enum EVlcCommond{
    EVLC_PLAY,          //播放
    EVLC_PAUSE,         //暂停
    EVLC_STOP,          //停止
    EVLC_GET_VOLUME,    //获取音量
    EVLC_GET_POSITION   //获取进度
};

class VideoClientController
{
public:
    Ui::MainWindow* m_ui;         //视图层
    EVlc* m_evlc;               //模型层
private:
    MainWindow* m_mainwindow;
public:
    //获取实例
    static VideoClientController* GetInstance();
    //初始化视图层和模型层
    int Init();
    //应用运行
    int Invoke();
    //设置媒体路径
    int SetMedia(std::string strUrl);
    //控制客户端功能
    float VideoCtrl(EVlcCommond cmd);
    //控制播放进度
    void SetPostion(float pos);
    //设置播放窗口
    void SetWidget(QWidget*& widget);
    //设置音量大小
    void SetVolume(int volume);
    //获取Mediainfo
    VSize GetMediaInfo();
private:
    //单例模型
    VideoClientController();
    ~VideoClientController();
    VideoClientController(const VideoClientController& s);
    VideoClientController& operator=(const VideoClientController& s);
    static VideoClientController* instance;
};

#endif // VIDEOCLIENTCONTROLLER_H
