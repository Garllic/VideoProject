#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class VideoClientController;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTimer *m_timer;    //定时更新播放进度状态
protected:
    VideoClientController* c;

public:
    MainWindow(QWidget *parent = nullptr);
    Ui::MainWindow* GetUi();
    void SetDuration(float t);
    void TimerStart();
    void TimerStop();
    void ResetPosition();
    ~MainWindow();
private slots:
    //播放按钮点击
    void on_BTN_Play_clicked();
    //停止按钮点击
    void on_BTN_Stop_clicked();
    //音量滑动条改变
    void on_VS_Volume_valueChanged(int value);
    //退出
    void on_BTN_Exit_clicked();
    //更新播放进度状态
    void updata_duration();
    //操作滑动条改变播放进度
    void on_HS_Position_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
