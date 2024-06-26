#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoclientcontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    c = VideoClientController::GetInstance();

    //定义定时器，并绑定槽函数
    m_timer = new QTimer(this);
    m_timer->stop();
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setInterval(300);
    m_timer->setSingleShot(false);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updata_duration()));

}

Ui::MainWindow *MainWindow::GetUi()
{
    return ui;
}

void MainWindow::SetDuration(float t)
{
    std::string time = std::to_string(t);
    ui->LB_Duration->setText(QString::fromStdString(time));
}

void MainWindow::TimerStart()
{
    m_timer->start();
}

void MainWindow::TimerStop()
{
    m_timer->stop();
}

void MainWindow::ResetPosition()
{
    ui->HS_Position->setValue(0);
    ui->LB_Duration->setText(QString("00:00:00"));
    ui->LB_Pos->setText(QString("00:00:00"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_timer;
}

void MainWindow::on_BTN_Play_clicked()
{
    //按钮会在点击后自动切换（播放/暂停）
    if(c->m_isPlaying){
        ui->BTN_Play->setText("播放");
        //controller的暂停接口
        c->VideoCtrl(EVLC_PAUSE);
    }else {
        ui->BTN_Play->setText("暂停");
        //controller的播放接口
        std::string url = ui->EDIT_URL->toPlainText().toStdString();
        if(url != c->m_url){
            c->SetMedia(url);
            c->SetWidget(ui->WGT_Player);
        }
        //更新音量
        c->SetVolume(ui->VS_Volume->value());
        //开始播放
        c->VideoCtrl(EVLC_PLAY);
        //更新总时长
        long long t = c->m_duration/1000;
        QString time = QString("%1:%2:%3")
                           .arg(t/3600, 2, 10, QChar('0'))
                           .arg(t/60, 2, 10, QChar('0'))
                           .arg(t%60, 2, 10, QChar('0'));
        ui->LB_Duration->setText(time);
        ui->HS_Position->setMaximum(c->m_duration);
    }

}


void MainWindow::on_BTN_Stop_clicked()
{
    ui->BTN_Play->setText("播放");
    //controller的停止接口
    c->VideoCtrl(EVLC_STOP);
}

void MainWindow::on_VS_Volume_valueChanged(int value)
{
    //根据滑动条改变音量标签的显示
    std::string str = std::to_string(value);
    ui->LB_Volume->setText(QString::fromStdString(str));
    //改变音量大小
    c->SetVolume(value);
}


void MainWindow::on_BTN_Exit_clicked()
{
    this->close();
}

void MainWindow::updata_duration()
{
    float pos = c->VideoCtrl(EVLC_GET_POSITION);
    //获取当前视频进度
    long long t = pos * c->m_duration/1000;
    QString time = QString("%1:%2:%3")
                       .arg(t/3600, 2, 10, QChar('0'))
                       .arg(t/60, 2, 10, QChar('0'))
                       .arg(t%60, 2, 10, QChar('0'));
    //更新ui
    ui->LB_Pos->setText(time);
    ui->HS_Position->setValue(pos * c->m_duration);
}

void MainWindow::on_HS_Position_sliderMoved(int position)
{
    c->SetPostion(float(position)/c->m_duration);
}
