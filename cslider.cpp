#include "cslider.h"

CSlider::CSlider(QWidget *parent):QSlider(parent)
{

}

CSlider::~CSlider()
{

}

void CSlider::mousePressEvent(QMouseEvent *ev)
{
    int current, len;
    if(this->orientation() == Qt::Vertical){
        len = this->height();
        current = len - ev->pos().y();
    }else{
        current = ev->pos().x();
        len = this->width();
    }
    double per = current * 1.0 / len;
    int value = per * (this->maximum() - this->minimum()) + this->minimum();
    emit sliderMoved(value);
    this->setValue(value);
    QSlider::mousePressEvent(ev);
}
