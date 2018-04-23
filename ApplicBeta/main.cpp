#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName( "Circuit Engineering Technologies" );
    QApplication::setApplicationName( "HFTA" );

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
