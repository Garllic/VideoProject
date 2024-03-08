/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "cslider.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *WGT_Player;
    QGroupBox *groupBox_Volume;
    QVBoxLayout *verticalLayout;
    CSlider *VS_Volume;
    QLabel *LB_Volume;
    QGroupBox *verticalGroupBox;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_URL;
    QHBoxLayout *horizontalLayout_3;
    QTextEdit *EDIT_URL;
    QGroupBox *groupBox_Pos;
    QHBoxLayout *horizontalLayout_2;
    QLabel *LB_Pos;
    CSlider *HS_Position;
    QLabel *LB_Duration;
    QGroupBox *groupBox_BTN;
    QHBoxLayout *horizontalLayout_BTN;
    QPushButton *BTN_Play;
    QPushButton *BTN_Stop;
    QPushButton *BTN_Exit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1198, 772);
        MainWindow->setAutoFillBackground(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        WGT_Player = new QWidget(centralwidget);
        WGT_Player->setObjectName("WGT_Player");
        WGT_Player->setGeometry(QRect(0, 0, 1201, 591));
        WGT_Player->setAutoFillBackground(false);
        WGT_Player->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        groupBox_Volume = new QGroupBox(centralwidget);
        groupBox_Volume->setObjectName("groupBox_Volume");
        groupBox_Volume->setGeometry(QRect(1070, 610, 71, 141));
        groupBox_Volume->setAlignment(Qt::AlignCenter);
        verticalLayout = new QVBoxLayout(groupBox_Volume);
        verticalLayout->setObjectName("verticalLayout");
        VS_Volume = new CSlider(groupBox_Volume);
        VS_Volume->setObjectName("VS_Volume");
        VS_Volume->setMaximum(100);
        VS_Volume->setPageStep(10);
        VS_Volume->setValue(100);
        VS_Volume->setOrientation(Qt::Vertical);
        VS_Volume->setInvertedAppearance(false);
        VS_Volume->setInvertedControls(false);
        VS_Volume->setTickPosition(QSlider::TicksAbove);
        VS_Volume->setTickInterval(10);

        verticalLayout->addWidget(VS_Volume, 0, Qt::AlignHCenter);

        LB_Volume = new QLabel(groupBox_Volume);
        LB_Volume->setObjectName("LB_Volume");

        verticalLayout->addWidget(LB_Volume, 0, Qt::AlignHCenter);

        verticalGroupBox = new QGroupBox(centralwidget);
        verticalGroupBox->setObjectName("verticalGroupBox");
        verticalGroupBox->setGeometry(QRect(190, 600, 791, 161));
        verticalLayout_2 = new QVBoxLayout(verticalGroupBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(1, -1, 1, 1);
        groupBox_URL = new QGroupBox(verticalGroupBox);
        groupBox_URL->setObjectName("groupBox_URL");
        QFont font;
        font.setPointSize(8);
        groupBox_URL->setFont(font);
        groupBox_URL->setAcceptDrops(false);
        horizontalLayout_3 = new QHBoxLayout(groupBox_URL);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        EDIT_URL = new QTextEdit(groupBox_URL);
        EDIT_URL->setObjectName("EDIT_URL");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setItalic(false);
        EDIT_URL->setFont(font1);
        EDIT_URL->setStyleSheet(QString::fromUtf8("font: 10pt \"Microsoft YaHei UI\";"));

        horizontalLayout_3->addWidget(EDIT_URL);


        verticalLayout_2->addWidget(groupBox_URL);

        groupBox_Pos = new QGroupBox(verticalGroupBox);
        groupBox_Pos->setObjectName("groupBox_Pos");
        horizontalLayout_2 = new QHBoxLayout(groupBox_Pos);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        LB_Pos = new QLabel(groupBox_Pos);
        LB_Pos->setObjectName("LB_Pos");

        horizontalLayout_2->addWidget(LB_Pos);

        HS_Position = new CSlider(groupBox_Pos);
        HS_Position->setObjectName("HS_Position");
        HS_Position->setMaximum(100);
        HS_Position->setSingleStep(1);
        HS_Position->setPageStep(1);
        HS_Position->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(HS_Position);

        LB_Duration = new QLabel(groupBox_Pos);
        LB_Duration->setObjectName("LB_Duration");

        horizontalLayout_2->addWidget(LB_Duration);


        verticalLayout_2->addWidget(groupBox_Pos);

        groupBox_BTN = new QGroupBox(verticalGroupBox);
        groupBox_BTN->setObjectName("groupBox_BTN");
        horizontalLayout_BTN = new QHBoxLayout(groupBox_BTN);
        horizontalLayout_BTN->setObjectName("horizontalLayout_BTN");
        BTN_Play = new QPushButton(groupBox_BTN);
        BTN_Play->setObjectName("BTN_Play");

        horizontalLayout_BTN->addWidget(BTN_Play);

        BTN_Stop = new QPushButton(groupBox_BTN);
        BTN_Stop->setObjectName("BTN_Stop");

        horizontalLayout_BTN->addWidget(BTN_Stop);

        BTN_Exit = new QPushButton(groupBox_BTN);
        BTN_Exit->setObjectName("BTN_Exit");

        horizontalLayout_BTN->addWidget(BTN_Exit);


        verticalLayout_2->addWidget(groupBox_BTN);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\222\255\346\224\276\345\231\250", nullptr));
        groupBox_Volume->setTitle(QString());
        LB_Volume->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        groupBox_URL->setTitle(QCoreApplication::translate("MainWindow", "\346\222\255\346\224\276\345\234\260\345\235\200", nullptr));
        EDIT_URL->setMarkdown(QCoreApplication::translate("MainWindow", "rtsp://127.0.0.1:554/live\n"
"\n"
"", nullptr));
        EDIT_URL->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:6px; margin-bottom:6px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">rtsp://127.0.0.1:554/live</p></body></html>", nullptr));
        groupBox_Pos->setTitle(QString());
        LB_Pos->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        LB_Duration->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        BTN_Play->setText(QCoreApplication::translate("MainWindow", "\346\222\255\346\224\276", nullptr));
        BTN_Stop->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242", nullptr));
        BTN_Exit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
