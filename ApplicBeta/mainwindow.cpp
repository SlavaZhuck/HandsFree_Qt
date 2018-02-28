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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setMaxLength(32);
    ui->progressBar->setValue(0);

    //Показать доступнуе COM-порты------------------------------------------------------------
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxCom->addItem(info.portName());
    }

    connect(ui->comboBoxBaudRate, SIGNAL(currentIndexChanged(int)), this, SLOT(checkCustomBaudRatePolicy(int)));

    ui->comboBoxBaudRate->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->comboBoxBaudRate->addItem(QLatin1String("38400"),  QSerialPort::Baud38400);
    ui->comboBoxBaudRate->addItem(QLatin1String("19200"),  QSerialPort::Baud19200);
    ui->comboBoxBaudRate->addItem(QLatin1String("9600"),   QSerialPort::Baud9600);
    ui->comboBoxBaudRate->addItem(QLatin1String("Custom"));
    // Длина потока
    ui->comboBoxDataBits->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->comboBoxDataBits->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->comboBoxDataBits->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->comboBoxDataBits->addItem(QLatin1String("8"), QSerialPort::Data8);
    ui->comboBoxDataBits->setCurrentIndex(3);
    // Определение бита четности
    ui->comboBoxParity->addItem(QLatin1String("None"),  QSerialPort::NoParity);
    ui->comboBoxParity->addItem(QLatin1String("Even"),  QSerialPort::EvenParity);
    ui->comboBoxParity->addItem(QLatin1String("Odd"),   QSerialPort::OddParity);
    ui->comboBoxParity->addItem(QLatin1String("Mark"),  QSerialPort::MarkParity);
    ui->comboBoxParity->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);
    // Определение стопового бита
    ui->comboBoxStopBits->addItem(QLatin1String("1"),   QSerialPort::OneStop);
    #ifdef Q_OS_WIN
    ui->comboBoxStopBits->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
    #endif
    ui->comboBoxStopBits->addItem(QLatin1String("2"),   QSerialPort::TwoStop);
    // Контроль потока
    ui->comboBoxFlowControl->addItem(QLatin1String("None"),     QSerialPort::NoFlowControl);
    ui->comboBoxFlowControl->addItem(QLatin1String("RTS/CTS"),  QSerialPort::HardwareControl);
    ui->comboBoxFlowControl->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);

    //Отправить посылку записанную в QLineEdit (текстовое поле)
    connect(ui->pushButton_2,SIGNAL(clicked()),this, SLOT(on_lineEdit_returnPressed()));

    //Формирование потока
    QThread *thread_New = new QThread;//Создаем поток для порта гарнитуры
    Port *PortNew = new Port();//Создаем обьект по классу
    PortNew->moveToThread(thread_New);//Помещаем класс  в поток    
    PortNew->thisPort.moveToThread(thread_New);//Помещаем сам порт в поток
    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));//Переназначения метода run
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));//Переназначение метод выхода
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));//Удалить поток нафиг
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));//Удалить поток нафиг
    connect(this,SIGNAL(savesettings(QString,int,int,int,int,int)),PortNew,SLOT(Write_Settings_Port(QString,int,int,int,int,int)));//Слот - ввод настроек
    connect(ui->pushButton_5, SIGNAL(clicked()),PortNew,SLOT(ConnectPort()));//Подключение порта
    connect(ui->pushButton_6, SIGNAL(clicked()),PortNew,SLOT(DisconnectPort()));//Отключение порта
    connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(Print(QString)));//Лог ошибок
    connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));//Запись в порт по готовности
    connect(PortNew, SIGNAL(ReadInPort()),this,SLOT(Print(QByteArray)));//Отображение принятой обработанной посылки
    connect(this, SIGNAL(readyRead()), PortNew, SLOT(ReadInPort()));//подключаем   чтение с порта по сигналу readyRead()
    connect(this, SIGNAL(error(QSerialPort::SerialPortError)),PortNew,SLOT(handleError(QSerialPort::SerialPortError)));//Сообщение об ошибке
    connect(PortNew, SIGNAL(sendParam()),this, SLOT(MacAdr()));//Отображение МАС адреса гарнитуры
    thread_New->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Сгенеррировать ключ и сформировать посылку--------------------------------------
QByteArray buf_DataTxK;//массив посылки ключа
void MainWindow::on_pushButton_clicked()
{
    int i, j, k;
    ui->lineEdit->clear();
    QByteArray arr(16,0);    //массив для записи случайных чисел
    QByteArray DataTxC(19,0);//массив для рассчета CRC посылки
    QByteArray DataTxK(22,0);//посылка
    quint16 crc;

    DataTxK[0] = SB;//стартовый байт
    DataTxK[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));//адрес
    DataTxK[2] = DataTxC[1] = 0x10;       //Длина посылки
    DataTxK[3] = DataTxC[2] = SEND_FH_KEY;//Ctrl

    for (i = 0, j = 4, k = 3; i < 16; i++, j++, k++)
        {
            arr[i] = qrand()%(0 - 255 + 1)+ 0; //формирование массива случайных чисел
            DataTxK[j] = DataTxC[k] = arr[i];  //формирование посылки
        }

    crc = Crc16(DataTxC, 19);//Расчет CRC
    DataTxK[20] = (crc & 0xFF00)>>8;
    DataTxK[21] = crc & 0x00FF;

    for (i = 0; i < 22; i++)
        {
            buf_DataTxK[i] = DataTxK[i];//формирование посылки
        }
    qDebug()<<arr.toHex();//отображение в дебагере только ключа без формирования посылки
    ui->lineEdit->setText(QByteArray(arr.constData()).toHex().toUpper());//Отображение только ключа без формирования посылки
}

//Запрос статуса устройства-----------------------------------------------------
void MainWindow::on_pushButton_3_clicked()
{
    QByteArray DataTxC(6,0);//массив для рассчета CRC посылки
    QByteArray DataTx(9,0); //массив данных
    quint16 crc;

    DataTx[0] = SB;//стартовый байт
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));//адрес
    DataTx[2] = DataTxC[1] = 0x03;      //Длина посылки
    DataTx[3] = DataTxC[2] = GET_FH_KEY;//Ctrl
    DataTx[4] = DataTxC[3] = 0x01;
    DataTx[5] = DataTxC[4] = 0x02;
    DataTx[6] = DataTxC[5] = 0x03;
    crc = Crc16(DataTxC, 6);//Расчет CRC
    DataTx[7] = (crc & 0xFF00)>>8;
    DataTx[8] = crc & 0x00FF;
    Print("Прочитать ключ"); // Вывод данных в консоль
    writeData(DataTx);       //Запись в порт
    readyRead();
    qDebug()<<DataTx.toHex().toUpper();//Отображение в дебагере

    //ui->lineEdit->setText(QString(DataTx.toHex().constData()));
    //ui->lineEdit->setText(QString(DataTx.constData()));//toHex().
}

//Обновление параметров----------------------------------------------------------------------
void MainWindow::on_pushButton_4_clicked()
{
    ui->comboBoxCom->clear();

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxCom->addItem(info.portName());
    }
}

//Сохранение параметров порта----------------------------------------------------------------
void MainWindow::on_pushButton_5_clicked()
{
    savesettings(ui->comboBoxCom->currentText(),
                 ui->comboBoxBaudRate->currentText().toInt(),
                 ui->comboBoxDataBits->currentText().toInt(),
                 ui->comboBoxParity->currentText().toInt(),
                 ui->comboBoxStopBits->currentText().toInt(),
                 ui->comboBoxFlowControl->currentText().toInt());
    QTimer *ptimer = new QTimer();
    connect(ptimer, SIGNAL(timeout()),this, SLOT(TimerStart()));
    ptimer->start(6000);
}

//Формирование посылки в QLine Edit-----------------------------------------------------------
void MainWindow::on_lineEdit_returnPressed()
{    
    QString input = ui->lineEdit->text();// Присвоение "input" значения из EnterText
    QString input_spaces;//Для вставки пробелов
    QByteArray senddata_crc;//Для расчета CRC
    QByteArray senddata;
    QByteArray DataTxK;
    quint16 crc;

    if(input.length() < 31)
    {
        for(int i = input.length(); i < 32; i++)
            input += '0';
    }

    if(input[2] != " ")//если третий символ не пробел
    {
        for(int i = 0; i < input.length(); i = i + 3)
            input_spaces = input.insert(i, ' ');//вставляем пробелы в нужные маста
        input = input_spaces.remove(0, 1);//удаляем нулевой пробел
    }

    QStringList list1 = input.split(" ");//Для разделения на байты по пробелу
    int size = list1.size();
    for (int i = 0 ; i < size; i++)// Разделение по пробелу
    {
        QString ran = list1 [i];
        bool ok;
        senddata[i] = ran.toInt(&ok,16);
    }

    DataTxK[0] = SB;//стартовый байт
    DataTxK[1] = senddata_crc[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));//адрес
    DataTxK[2] = senddata_crc[1] = 0x10;//Длина посылки
    DataTxK[3] = senddata_crc[2] = SEND_FH_KEY;//Ctrl

    for (int i = 0, j = 4, k = 3; i < 16; i++, j++, k++)
        {
            DataTxK[j] = senddata_crc[k] = senddata[i];//формирование посылки
        }

    crc = Crc16(senddata_crc, 19);//Расчет CRC
    DataTxK[20] = (crc & 0xFF00)>>8;
    DataTxK[21] = crc & 0x00FF;
    Print("\rЗагрузка ключа в гарнитуру"); // Вывод данных в консоль
    writeData(DataTxK);//Запись в порт
    readyRead();//Порт готов к чтению

}

//Выбор скорости------------------------------------------------------------------------------
void MainWindow::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->comboBoxBaudRate->itemData(idx).isValid();
    ui->comboBoxBaudRate->setEditable(isCustomBaudRate);
    if(isCustomBaudRate)
    {
        ui->comboBoxBaudRate->clearEditText();
    }
}

//Функция вывода на консоль-----------------------------------------------------------------
void MainWindow::Print(QString data)
{
    ui->plainTextEdit->textCursor().insertText(QString(data) + '\r');
    ui->plainTextEdit->moveCursor(QTextCursor::End);
}

//Расчет CRC---------------------------------------------------------------------------------
quint16 MainWindow::Crc16(QByteArray pcBlock, quint16 len)
{
    unsigned short crc = 0xFFFF;
    unsigned char i;
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

//Отображение МАС адреса гарнитуры-----------------------------------------------
void MainWindow::MacAdr()
{
    QByteArray param = PortNew->ParamsGet();//Получаем значение param
    QString para_str(param.toHex().toUpper());
    for(int i = 0; i < para_str.length(); i = i + 3)
        para_str =para_str.insert(i, ':');  //вставляем ":" в нужные маста
    para_str = para_str.remove(0, 1);       //удаляем нулевой символ
    qDebug()<<param.toHex().toUpper();

    ui->label->setText("МАС адрес гарнитуры: "+ para_str);
    GetBatter();
}

//Запрос таймером параметров----------------------------------------------------
void MainWindow::TimerStart()
{
    QByteArray DataTxC(6,0);//Массив для расчета CRC
    QByteArray DataTx(9,0); //Массив данных
    quint16 crc;

    DataTx[0] = SB;         //Стартовый байт
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));//Адрес
    DataTx[2] = DataTxC[1] = 0x03;        //Длина данных
    DataTx[3] = DataTxC[2] = GET_FH_PARAM;//Ctrl
    DataTx[4] = DataTxC[3] = 0x01;
    DataTx[5] = DataTxC[4] = 0x02;
    DataTx[6] = DataTxC[5] = 0x03;
    crc = Crc16(DataTxC, 6);      //расчет crc отправляемой посылки
    DataTx[7] = (crc & 0xFF00)>>8;//расчет crc отправляемой посылки (старший байт)
    DataTx[8] = crc & 0x00FF;     //расчет crc отправляемой посылки (дладший байт)
    writeData(DataTx);//Запись в порт
    readyRead();      //Порт готов к чтению
    GetBatter();
}


//Расчет заряда батареи гарнитуры-----------------------------------------------
void MainWindow::GetBatter()
{
    QByteArray batter = PortNew->BatterGet();//Получаем значение batter

    int    per_cent;   //Проценты
    double per_cent_fl;//Проценты

    unsigned short val_bat = 0xFFFF;             //Переменная для перевода байт в ushort
    unsigned short bat_1 = batter.at(0);         //Нулевой элемент массива
    unsigned short bat_2 = batter.at(1) & 0x00ff;//Первый элемент массива

    val_bat = ((bat_1 << 8) | bat_2) & 0xffff;//Загоняем масив в байты
    per_cent_fl = ((100*(val_bat - 0x0A8C))/(0x109A - 0x0A8C));//Получаем уровень заряда в процентах (per_cent_fl = ((100*(val_volt - 2.7))/(4.25 - 2.7)))
    per_cent = (int)(per_cent_fl + 0.5);      //Округляем до целых

    ui->progressBar->setValue(per_cent);      //Подставляем в ProgressBar
}
