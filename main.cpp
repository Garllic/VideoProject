#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "videoclientcontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "weather_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    VideoClientController* controller = VideoClientController::GetInstance();
    if(controller->Init() != 0){
        return -1;
    }
    controller->Invoke();

    return a.exec();
}
