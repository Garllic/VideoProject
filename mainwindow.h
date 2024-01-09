#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class VideoClientController;

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    VideoClientController* c;

public:
    MainWindow(QWidget *parent = nullptr);
    Ui::MainWindow* GetUi();
    ~MainWindow();
private slots:
    //播放按钮点击
    void on_BTN_Play_clicked();
    //停止按钮点击
    void on_BTN_Stop_clicked();
    //音量滑动条改变
    void on_VS_Volume_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
