#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        //TODO:controller的暂停接口
    }else {
        ui->BTN_Play->setText("暂停");
        //TODO:controller的播放接口
    }

}


void MainWindow::on_BTN_Pause_clicked()
{
    ui->BTN_Play->setText("播放");
    //TODO:controller的停止接口
}

void MainWindow::on_VS_Volume_valueChanged(int value)
{
    //根据滑动条改变音量标签的显示
    std::string str = std::to_string(value);
    ui->LB_Volume->setText(QString::fromStdString(str));
}

