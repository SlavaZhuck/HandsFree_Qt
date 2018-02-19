#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "port.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void writeData(QByteArray data);

    void savesettings(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);

    void readyRead (void);

    void error(QSerialPort::SerialPortError);

private slots:
    void on_pushButton_4_clicked();

    //void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

    void checkCustomBaudRatePolicy(int idx);

    void Print(QString data);

    void on_lineEdit_returnPressed();    

    quint16 Crc16(QByteArray pcBlock, quint16 len);

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    //void showPortInfo(int idx);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    Port *PortNew;
};

#endif // MAINWINDOW_H
