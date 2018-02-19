#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName( "Circuit Engineering Technologies" );
    QApplication::setApplicationName( "AplicBeta" );

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//test
    return a.exec();
}
