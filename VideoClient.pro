QT       += core gui

DEPENDPATH += $$PWD/include
LIBS += $$PWD/lib/libvlc.lib
LIBS += $$PWD/lib/libvlccore.lib
INCLUDEPATH += $$PWD/include



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    evlc.cpp \
    main.cpp \
    mainwindow.cpp \
    videoclientcontroller.cpp

HEADERS += \
    evlc.h \
    mainwindow.h \
    videoclientcontroller.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    VideoClient_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc


