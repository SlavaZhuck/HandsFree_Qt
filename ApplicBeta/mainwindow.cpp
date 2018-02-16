#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include <QString>
#include <QScreen>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QIODevice>
#include <QThread>
#include <error.h>
#include <unistd.h>
#include <QMetaEnum>
#include <QDebug>
#include <Random.h>
#include <QTextCodec>


#define GET_STATUS     0x01 // запрос статуса устройства на линии

#define SEND_DATA      0x02 // передача массива данных
#define SEND_FH_PARAM  0x03 // передача текущих параметров гарнитуры
#define SEND_FH_KEY    0x04 // передача ключа шифрования
#define SEND_FH_CR_TP  0x05 // передача типа шифрования

#define GET_FH_PARAM   0x23 // запрос текущих параметров гарнитуры
#define GET_FH_KEY     0x24 // запрос ключа шифрования
#define GET_FH_CR_TP   0x25 // запрос типа шифрования

#define REC_OK         0x12 // подтверждение безошибочного приёма данных
#define REC_ERROR      0x13 // в процессе приёма возникли ошибки
#define STATUS_OK      0x10 // устройство работает в штатном режиме
#define STATUS_BAD     0x11 // устройство работает некорректно
#define NO_COMAND      0x14 // принятой команды нет в списке

#define SB             0xaa

#define ADR_PC         0x1
#define ADR_HF         0x3
#define ADR_TX(x)      x<<4
#define ADR_REC(x)     x

unsigned char buf_DataTxK[20];//массив посылки ключа

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        ui->comboBoxCom->addItem(info.portName());

    connect(ui->comboBoxBaudRate, SIGNAL(currentIndexChanged(int)) ,this, SLOT(checkCustomBaudRatePolicy(int)));

    ui->comboBoxBaudRate->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->comboBoxBaudRate->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->comboBoxBaudRate->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->comboBoxBaudRate->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->comboBoxBaudRate->addItem(QLatin1String("Custom"));
   // fill data bits
    ui->comboBoxDataBits->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->comboBoxDataBits->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->comboBoxDataBits->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->comboBoxDataBits->addItem(QLatin1String("8"), QSerialPort::Data8);
    ui->comboBoxDataBits->setCurrentIndex(3);
   // fill parity
    ui->comboBoxParity->addItem(QLatin1String("None"), QSerialPort::NoParity);
    ui->comboBoxParity->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    ui->comboBoxParity->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    ui->comboBoxParity->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    ui->comboBoxParity->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);
   // fill stop bits
    ui->comboBoxStopBits->addItem(QLatin1String("1"), QSerialPort::OneStop);
    #ifdef Q_OS_WIN
    ui->comboBoxStopBits->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
    #endif
    ui->comboBoxStopBits->addItem(QLatin1String("2"), QSerialPort::TwoStop);
   // fill flow control
    ui->comboBoxFlowControl->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    ui->comboBoxFlowControl->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    ui->comboBoxFlowControl->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);

    connect(ui->pushButton_2,SIGNAL(clicked()),this, SLOT(on_lineEdit_returnPressed()));
    //connect(ui->pushButton_2,SIGNAL(clicked()),this, SLOT(send_key(QByteArray)));

    //Формирование потока
    QThread *thread_New = new QThread;//Создаем поток для порта платы
    Port *PortNew = new Port();//Создаем обьект по классу
    PortNew->moveToThread(thread_New);//помешаем класс  в поток
    PortNew->thisPort.moveToThread(thread_New);//Помещаем сам порт в поток
    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));//Переназначения метода run
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));//Переназначение метода выход
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));//Удалить поток нафиг
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));//Удалить поток нафиг
    connect(this,SIGNAL(savesettings(QString,int,int,int,int,int)),PortNew,SLOT(Write_Settings_Port(QString,int,int,int,int,int)));//Слот - ввод настроек
    //connect(ui->checkBox, SIGNAL(clicked()), this, SLOT(on_checkBox_stateChanged(int arg1)));
    //connect(ui->checkBox, SIGNAL(clicked()), PortNew,SLOT(ConnectPort()));
    connect(ui->pushButton_5, SIGNAL(clicked()),PortNew,SLOT(ConnectPort()));
    connect(ui->pushButton_6, SIGNAL(clicked()),PortNew,SLOT(DisconnectPort()));
    connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));
    connect(PortNew, SIGNAL(ReadInPort()),this,SLOT(Print(QByteArray)));
    connect(ui->pushButton_2,SIGNAL(clicked()),this, SLOT(send_key(QByteArray)));
    thread_New->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//Обновление параметров
void MainWindow::on_pushButton_4_clicked()
{
    ui->comboBoxCom->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxCom->addItem(info.portName());
    }
}

void MainWindow::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->comboBoxBaudRate->itemData(idx).isValid();
    ui->comboBoxBaudRate->setEditable(isCustomBaudRate);
    if(isCustomBaudRate)
    {
        ui->comboBoxBaudRate->clearEditText();
    }
}

void MainWindow::on_lineEdit_returnPressed()
{
    QByteArray data; // Текстовая переменная
    QString input = ui->lineEdit->text();
    QStringList list1 = input.split(" ");
    data = ui->lineEdit->text().toLocal8Bit().toHex() + '\r'; // Присвоение "data" значения из EnterText
    QByteArray senddata;
    int size = list1.size();
    for (int i=0; i < size; i++)
    {
        QString ran = list1 [i];
        bool ok;
        senddata[i]=ran.toInt(&ok,16);
    }
    writeData(senddata); // Отправка данных в порт
    Print(senddata); // Вывод данных в консоль
}

void MainWindow::Print(QString data)  //QString data
{
    ui->plainTextEdit->textCursor().insertText(QString(data) + '\r');
    ui->plainTextEdit->moveCursor(QTextCursor::End);
}

void MainWindow::on_pushButton_7_clicked()
{
    savesettings(ui->comboBoxCom->currentText(), ui->comboBoxBaudRate->currentText().toInt(),ui->comboBoxDataBits->currentText().toInt(),
                 ui->comboBoxParity->currentText().toInt(), ui->comboBoxStopBits->currentText().toInt(), ui->comboBoxFlowControl->currentText().toInt());

}
//расчет CRC
quint16 MainWindow::Crc16(QByteArray pcBlock, quint16 len)
{
    unsigned short crc =0xFFFF;
    unsigned char i;
    //quint8 buf[25];
    unsigned char buf[len];

    for(int j = 0; j < len; j++)
    {
        buf[j] = pcBlock[j];
    }

    for (int j = 0; j < len; j++)
        {
            crc ^= buf[j]<<8;

            for (i =0; i < 8; i++)
            {
                crc = crc & 0x8000 ? (crc<<1) ^0x1021: crc <<1;
            }
        }
    return crc;
}
//Запрос статуса устройства
void MainWindow::on_pushButton_3_clicked()
{
    QByteArray DataTxC(6,0);
    QByteArray DataTx(9,0);
    int i = 0;
    quint16 crc;
    DataTx[0] = SB;
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));
    DataTx[2] = DataTxC[1] = 0x03;
    DataTx[3] = DataTxC[2] = GET_FH_KEY;
    DataTx[4] = DataTxC[3] = 0x01;
    DataTx[5] = DataTxC[4] = 0x02;
    DataTx[6] = DataTxC[5] = 0x03;
    crc = Crc16(DataTxC, 6);
    DataTx[7] = (crc & 0xFF00)>>8;
    DataTx[8] = crc & 0x00FF;

    for(i = 0; i < 9; i++)
    {
        qDebug()<<DataTx[i];
    }
     ui->lineEdit->setText(QString(DataTx.toHex().constData()));
    //ui->lineEdit->setText(QString(DataTx.constData()));//toHex().
}

//Сгенеррировать ключ и сформировать посылку
void MainWindow::on_pushButton_clicked()
{
    int i, j, k;
    ui->lineEdit->clear();
    QByteArray arr(16,0);//массив для записи случайных чисел
    QByteArray DataTxC(19,0);//массив для рассчета CRC посылки
    QByteArray DataTxK(22,0);//посылка
    quint16 crc;

    DataTxK[0] = SB;//стартовый байт
    DataTxK[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));//адрес
    DataTxK[2] = DataTxC[1] = 0x10;
    DataTxK[3] = DataTxC[2] = SEND_FH_KEY;

    for (i = 0, j = 4, k = 3; i < 16; i++, j++, k++)
        {
            arr[i] = Random::get(10, 99 );//формирование массива случайных чисел
            DataTxK[j] = DataTxC[k] = arr[i];//формирование посылки
            //qDebug()<<arr.toHex();//отображение в дебагере
        }

    crc = Crc16(DataTxC, 18);
    DataTxK[19] = (crc & 0xFF00)>>8;
    DataTxK[20] = crc & 0x00FF;

//    QString str(DataTxK);
//    qDebug() << str;
//    str.replace(QRegExp("(..)[^$]"), QString("\\1 "));
//    qDebug() << str;
//    ui->lineEdit->setText(QString(str.constData()).toUpper().toLocal8Bit().toHex());

    for (i = 0; i < 22; i++)
        {

            buf_DataTxK[i] = DataTxK[i];//формирование посылки
            //qDebug()<<arr.toHex();//отображение в дебагере
        }
    ui->lineEdit->setText(QByteArray(DataTxK.constData()).toUpper().toHex());
}


void MainWindow::send_key(QByteArray data)
{
    QByteArray data_send;
    for (int i = 0; i < 22; i++)
        {

            data_send[i] = buf_DataTxK[i];//формирование посылки
            //qDebug()<<arr.toHex();//отображение в дебагере
        }
    writeData(data_send);
}















//void MainWindow::RedAnswer()
//{

//}

//void MainWindow::on_checkBox_stateChanged(int arg1)
//{
//    if(arg1)
//    {
//        savesettings(ui->comboBoxCom->currentText(), ui->comboBoxBaudRate->currentText().toInt(),ui->comboBoxDataBits->currentText().toInt(),
//                     ui->comboBoxParity->currentText().toInt(), ui->comboBoxStopBits->currentText().toInt(), ui->comboBoxFlowControl->currentText().toInt());
//        //PortNew.ConnectPort();
//        serial->open(QSerialPort::ReadWrite);
//        ui->comboBoxCom->setDisabled(true);
//        ui->comboBoxBaudRate->setDisabled(true);
//        ui->comboBoxDataBits->setDisabled(true);
//        ui->comboBoxFlowControl->setDisabled(true);
//        ui->comboBoxParity->setDisabled(true);
//        ui->comboBoxStopBits->setDisabled(true);
//        ui->pushButton_4->setDisabled(true);
//        ui->checkBox->setStyleSheet("background-color: green");
//        ui->checkBox->setText("  ВКЛЮЧЕНО");

//    }
//    else
//    {
//        //PortNew.DisconnectPort();
//        serial->close();

//        ui->comboBoxCom->setEnabled(true);
//        ui->comboBoxBaudRate->setEnabled(true);
//        ui->comboBoxDataBits->setEnabled(true);
//        ui->comboBoxFlowControl->setEnabled(true);
//        ui->comboBoxParity->setEnabled(true);
//        ui->comboBoxStopBits->setEnabled(true);
//        ui->pushButton_4->setEnabled(true);
//        PortNew.DisconnectPort();
//        ui->checkBox->setStyleSheet("background-color: red");
//        ui->checkBox->setText("  ОТКЛЮЧЕНО");
//    }
//}

//void MainWindow::on_checkBox_toggled(bool checked)
//{
//    if(checked)
//    {
//        savesettings(ui->comboBoxCom->currentText(), ui->comboBoxBaudRate->currentText().toInt(),ui->comboBoxDataBits->currentText().toInt(),
//                     ui->comboBoxParity->currentText().toInt(), ui->comboBoxStopBits->currentText().toInt(), ui->comboBoxFlowControl->currentText().toInt());
//        PortNew.ConnectPort();
//        ui->comboBoxCom->setDisabled(true);
//        ui->comboBoxBaudRate->setDisabled(true);
//        ui->comboBoxDataBits->setDisabled(true);
//        ui->comboBoxFlowControl->setDisabled(true);
//        ui->comboBoxParity->setDisabled(true);
//        ui->comboBoxStopBits->setDisabled(true);
//        ui->pushButton_4->setDisabled(true);
//        ui->checkBox->setStyleSheet("background-color: green");
//        ui->checkBox->setText("  ВКЛЮЧЕНО");

//    }
//    else
//    {
//        ui->checkBox->setStyleSheet("background-color: red");
//        ui->checkBox->setText("  ОТКЛЮЧЕНО");
//        ui->comboBoxCom->setEnabled(true);
//        ui->comboBoxBaudRate->setEnabled(true);
//        ui->comboBoxDataBits->setEnabled(true);
//        ui->comboBoxFlowControl->setEnabled(true);
//        ui->comboBoxParity->setEnabled(true);
//        ui->comboBoxStopBits->setEnabled(true);
//        ui->pushButton_4->setEnabled(true);
//        PortNew.DisconnectPort();
//        ui->checkBox->setStyleSheet("background-color: red");
//        ui->checkBox->setText("  ОТКЛЮЧЕНО");
//    }
//}
