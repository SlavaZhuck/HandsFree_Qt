#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName( "Circuit Engenering Technologies" );
    QApplication::setOrganizationDomain( "___________" );
    QApplication::setApplicationName( "AplicBeta" );


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//test
    return a.exec();
}
