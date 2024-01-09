#include "evlc.h"

int EVlc::SetMedia(const std::string &strUrl)
{
    if(m_instance == NULL) return -1;
    m_media = libvlc_media_new_location(m_instance, strUrl.c_str());
    if(!m_media) return -2;
    m_player = libvlc_media_player_new_from_media(m_media);
    if(!m_player) return -3;

    return 0;
}

int EVlc::Play()
{
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return -1;

    return libvlc_media_player_play(m_player);
}

int EVlc::SetWidget(QWidget *widget)
{
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return -1;
    libvlc_media_player_set_hwnd(m_player, (HWND)widget->winId());

    return 0;
}

int EVlc::Pause()
{
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return -1;
    libvlc_media_player_pause(m_player);

    return 0;
}

int EVlc::Stop()
{
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return -1;
    libvlc_media_player_stop(m_player);

    return 0;
}

float EVlc::GetPosition()
{
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return -1;

    return libvlc_media_player_get_position(m_player);
}

int EVlc::SetPosition(float pos)
{
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return -1;
    libvlc_media_player_set_position(m_player, pos);

    return 0;
}

int EVlc::GetVolume()
{
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return -1;

    return libvlc_audio_get_volume(m_player);
}

int EVlc::SetVolume(int volume)
{
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return -1;

    return libvlc_audio_set_volume(m_player, volume);
}

VSize EVlc::GetMediaInfo()
{
    VSize res;
    if(m_instance == NULL || m_media == NULL || m_player==NULL) return res;
    libvlc_video_get_size(m_player, 0, (unsigned *)&res.nWidth, (unsigned *)&res.nHeight);
    res.tm = libvlc_media_get_duration(m_media);

    return res;
}

EVlc &EVlc::operator=(const EVlc &s)
{
    if(&s != this){
        m_instance = s.m_instance;
        m_media = s.m_media;
        m_player = s.m_player;
        reset = s.reset;
    }

    return *this;
}

EVlc::EVlc()
{
    reset = true;
    m_instance = libvlc_new(0, NULL);
    m_media = NULL;
    m_player = NULL;
}

EVlc::EVlc(const EVlc &s)
{
    if(&s != this){
        m_instance = s.m_instance;
        m_media = s.m_media;
        m_player = s.m_player;
        reset = s.reset;
    }
}

EVlc::~EVlc()
{
    if(m_player!=NULL){
        libvlc_media_player_release(m_player);
    }
    if(m_media!=NULL){
        libvlc_media_release(m_media);
    }
    if(m_instance!=NULL){
        libvlc_release(m_instance);
    }
}

