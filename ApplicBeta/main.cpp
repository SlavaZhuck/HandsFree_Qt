#include "mainwindow.h"
#include <unistd.h>
#include <iostream>
#include <QApplication>
#include <QFile>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    #ifdef Q_OS_UNIX
    if (geteuid() !=0)
    {
        std::cout << "Для корректной работы приложения\n"
                     "нужно обладать правами на чтение/запись в порт.\n"
                     "Запустите приложение в режиме 'sudo'!" << std::endl;
        //return -1;
    }
    #endif

    QApplication::setOrganizationName( "Circuit Engineering Technologies" );
    QApplication::setApplicationName( "HFTA" );
    QApplication a(argc, argv);

    //Подтягиваем шрифты
    QString fontPath = ":/fonts/LiberationSerif-Regular.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId != -1)
    {
        QFont font("Liberation Serif", 24, QFont::Normal, false);
        a.setFont(font);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
