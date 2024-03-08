#ifndef CSLIDER_H
#define CSLIDER_H

#include <QObject>
#include <QSlider>
#include <QMouseEvent>

//创建一个新的CSlider以重写mousePressEvent实现滑动条移动到鼠标点击位置
class CSlider : public QSlider
{
public:
    CSlider(QWidget *parent = nullptr);
    ~CSlider();
    void mousePressEvent(QMouseEvent *ev);
};

#endif // CSLIDER_H
