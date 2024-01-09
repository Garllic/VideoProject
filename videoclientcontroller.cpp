#include <ui_mainwindow.h>

#include "videoclientcontroller.h"

VideoClientController* VideoClientController::instance = new VideoClientController();   //实例化单例

VideoClientController *VideoClientController::GetInstance()
{
    return instance;
}

int VideoClientController::Init()
{
    //mainwindow初始化
    m_mainwindow = new MainWindow();
    m_ui = m_mainwindow->GetUi();
    //evlc单例初始化
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

    return 0;
}

float VideoClientController::VideoCtrl(EVlcCommond cmd)
{
    switch (cmd) {
    case EVLC_PLAY:
        m_evlc->Play();
        break;
    case EVLC_PAUSE:
        m_evlc->Pause();
        break;
    case EVLC_STOP:
        m_evlc->Stop();
        break;
    case EVLC_GET_POSITION:
        return m_evlc->GetPosition();
        break;
    case EVLC_GET_VOLUME:
        return m_evlc->GetVolume();
        break;
    default:
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

VideoClientController::VideoClientController()
{
    m_evlc = NULL;
    m_mainwindow = NULL;
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
    }
}

VideoClientController &VideoClientController::operator=(const VideoClientController &s)
{
    if(&s != this){
        m_mainwindow = s.m_mainwindow;
        m_evlc = s.m_evlc;
    }

    return *this;
}
