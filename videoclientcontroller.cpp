#include "videoclientcontroller.h"

VideoClientController* VideoClientController::instance = new VideoClientController();   //实例化单例
VideoClientController::GC VideoClientController::releaseInstance;

VideoClientController *VideoClientController::GetInstance()
{
    return instance;
}

int VideoClientController::Init()
{
    //mainwindow初始化
    m_mainwindow = new MainWindow();
    //evlc初始化
    m_evlc = new EVlc();

    return 0;
}

int VideoClientController::Invoke()
{
    m_mainwindow->show();

    return 0;
}

int VideoClientController::SetMedia(std::string strUrl)
{
    m_evlc->SetMedia(strUrl);
    m_url = strUrl;

    return 0;
}

float VideoClientController::VideoCtrl(EVlcCommond cmd)
{
    switch (cmd) {
    case EVLC_PLAY:
        //视频播放
        m_evlc->Play();
        m_isPlaying = true;
        m_mainwindow->TimerStart();
        Sleep(100);
        m_duration = m_evlc->GetDuration();
        break;
    case EVLC_PAUSE:
        //视频暂停
        m_evlc->Pause();
        m_isPlaying = false;
        break;
    case EVLC_STOP:
        //视频停止
        m_evlc->Stop();
        Reset();
        break;
    case EVLC_GET_POSITION:
        //获取视频进度
        return m_evlc->GetPosition();
        break;
    case EVLC_GET_VOLUME:
        //获取视频音量
        return m_evlc->GetVolume();
        break;
    case EVLC_GET_DURATION:
        //获取时长
        return m_duration;
        break;
    default:
        return -1;
        break;
    }

    return 0.0;
}

void VideoClientController::SetPostion(float pos)
{
    int ret = m_evlc->SetPosition(pos);
    if(ret){
        qDebug("%s, %d, ret = %d\n", __FILE__, __LINE__, ret);
    }
}

void VideoClientController::SetWidget(QWidget *&widget)
{
    int ret = m_evlc->SetWidget(widget);
    if(ret){
        qDebug("%s, %d, ret = %d\n", __FILE__, __LINE__, ret);
    }
}

void VideoClientController::SetVolume(int volume)
{
    int ret = m_evlc->SetVolume(volume);
    if(ret){
        qDebug("%s, %d, ret = %d\n", __FILE__, __LINE__, ret);
    }
}

VSize VideoClientController::GetMediaInfo()
{
    return m_evlc->GetMediaInfo();
}

void VideoClientController::Reset()
{
    m_isPlaying = false;
    m_mainwindow->TimerStop();
    m_duration = 0;
    m_url = "";
    m_mainwindow->ResetPosition();
}

VideoClientController::VideoClientController()
{
    m_evlc = NULL;
    m_mainwindow = NULL;
    m_isPlaying = false;
    m_duration = 0;
    m_url = "";
}

VideoClientController::~VideoClientController()
{
    delete m_mainwindow;
    delete m_evlc;
}

VideoClientController::VideoClientController(const VideoClientController &s)
{
    if(&s != this){
        m_mainwindow = s.m_mainwindow;
        m_evlc = s.m_evlc;
        m_isPlaying = s.m_isPlaying;
        m_duration = s.m_duration;
        m_url = s.m_url;
    }
}

VideoClientController &VideoClientController::operator=(const VideoClientController &s)
{
    if(&s != this){
        m_mainwindow = s.m_mainwindow;
        m_evlc = s.m_evlc;
        m_isPlaying = s.m_isPlaying;
        m_duration = s.m_duration;
        m_url = s.m_url;
    }

    return *this;
}

VideoClientController::GC::~GC()
{
    if(VideoClientController::instance==NULL){
        return ;
    }
    delete VideoClientController::instance;
    VideoClientController::instance = NULL;
}

