#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videoclientcontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    c = VideoClientController::GetInstance();
}

Ui::MainWindow *MainWindow::GetUi()
{
    return ui;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_BTN_Play_clicked()
{
    //获取当前播放状态
    bool status = true;
    if(ui->BTN_Play->text() == "播放")
        status = false;

    //按钮会在点击后自动切换（播放/暂停）
    if(status){
        ui->BTN_Play->setText("播放");
        //controller的暂停接口
        c->VideoCtrl(EVLC_PAUSE);
    }else {
        ui->BTN_Play->setText("暂停");
        //controller的播放接口
        if(c->m_evlc->reset){
            std::string url = ui->EDIT_URL->toPlainText().toStdString();
            if(c->SetMedia(url) == 0){
                c->SetWidget(ui->WGT_Player);
                c->m_evlc->reset = false;
            }
        }
        c->VideoCtrl(EVLC_PLAY);
    }

}


void MainWindow::on_BTN_Stop_clicked()
{
    ui->BTN_Play->setText("播放");
    //controller的停止接口
    c->VideoCtrl(EVLC_STOP);
    c->m_evlc->reset = true;
}

void MainWindow::on_VS_Volume_valueChanged(int value)
{
    //根据滑动条改变音量标签的显示
    std::string str = std::to_string(value);
    ui->LB_Volume->setText(QString::fromStdString(str));
}

