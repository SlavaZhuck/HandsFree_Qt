#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "port.h"

#define MAX_VAL_BAT  0x0772
#define MIN_VAl_BAT  0x04D0

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

    void sendParam();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void checkCustomBaudRatePolicy(int idx);

    void Print(QString data);

    void on_lineEdit_returnPressed();    

    quint16 Crc16(QByteArray pcBlock, quint16 len);

    void MacAdr();

    void timerStartStopSlot();

    void timerStopSlot();

    void TimerStart();

    void GetBatter();

private:
    Ui::MainWindow *ui;
    Port *PortNew;
    QTimer* ptimer;
};

#endif // MAINWINDOW_H
