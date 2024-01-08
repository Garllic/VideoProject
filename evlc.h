#ifndef EVLC_H
#define EVLC_H

#include <vlc/vlc.h>
#include <string>
#include <QWidget>

typedef struct {
    int nWidth;
    int nHeight;
    long long tm;
}VSize;

class EVlc
{
protected:
    libvlc_instance_t* m_instance;
    libvlc_media_t* m_media;
    libvlc_media_player_t* m_player;
public:
    static EVlc* GetInstance();
    int SetMedia(const std::string& strUrl);
    int Play();
    int SetWidget(QWidget* widget);
    int Pause();
    int Stop();
    float GetPosition();
    int SetPosition(float pos);
    int GetVolume();
    int SetVolume(int volume);
    VSize GetMediaInfo();
private:
    EVlc& operator=(const EVlc& s);
    EVlc();
    EVlc(const EVlc& s);
    ~EVlc();
    static EVlc* instance;
};

EVlc* EVlc::instance = new EVlc;

#endif // EVLC_H
