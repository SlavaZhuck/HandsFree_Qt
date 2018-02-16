#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName( "X" );
    QApplication::setOrganizationDomain( "___________" );
    QApplication::setApplicationName( "Y" );


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//test
    return a.exec();
}
