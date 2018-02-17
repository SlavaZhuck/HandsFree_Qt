#include "port.h"
#include <qdebug.h>
#include <mainwindow.h>
#include <QString>
#include <QTextCodec>
#include <QThread>

//QByteArray rx_buf[25];
//QByteArray tx_buf[25];

Port :: Port(QObject *parent) : QObject(parent)
{

}

Port :: ~Port()
{
    qDebug("OOps");//Виден в дебаге при отключении
    emit finished_Port();
}

void Port :: process_Port()
{
    qDebug("Hi");//виден в дебаге при подключении гарнитуры


}

//запись насроек
void Port :: Write_Settings_Port(QString name, int baudrate, int DataBits,
                                 int Parity, int StopBits, int FlowControl)
{
    SettingsPort.name        = name;
    SettingsPort.baudRate    = (QSerialPort::BaudRate)baudrate;
    SettingsPort.dataBits    = (QSerialPort::DataBits)DataBits;
    SettingsPort.parity      = (QSerialPort::Parity)Parity;
    SettingsPort.stopBits    = (QSerialPort::StopBits)StopBits;
    SettingsPort.flowControl = (QSerialPort::FlowControl)FlowControl;
}

//Подключение порта
void Port :: ConnectPort(void)
{
    thisPort.setPortName(SettingsPort.name);
    if(thisPort.open(QIODevice::ReadWrite))
    {
        if(thisPort.setBaudRate(SettingsPort.baudRate)
                && thisPort.setDataBits(SettingsPort.dataBits)
                && thisPort.setParity(SettingsPort.parity)
                && thisPort.setStopBits(SettingsPort.stopBits)
                && thisPort.setFlowControl(SettingsPort.flowControl))
        {
            if(thisPort.isOpen())
            {//если открыт - прнимаем надпись "Порт открыт"
                error_((SettingsPort.name+ "  Порт открыт:\r")/*.toLocal8Bit()*/);
                uint8_t status = 0;


                while(status != STATUS_OK){
                    QThread::sleep(1);
                    tx_get_status();
                    status = ReadInPort();
                }
                tx_get_fh_param();

                ReadInPort();
                tx_get_fh_key();
                ReadInPort();
            }
        }
        else
        {
            thisPort.close();
            error_(thisPort.errorString().toLocal8Bit());
        }
    }
    else
    {
        thisPort.close();
        error_(thisPort.errorString().toLocal8Bit());
    }    
}

void Port::handleError(QSerialPort::SerialPortError error)
{//выдает сообщение об ошибке
    if ( (thisPort.isOpen()) && (error == QSerialPort::ResourceError))
    {
        error_(thisPort.errorString().toLocal8Bit());
        DisconnectPort();
    }
}

//Отключение порта
void  Port::DisconnectPort()
{
    if(thisPort.isOpen())
    {
        thisPort.close();
        error_(SettingsPort.name.toLocal8Bit() + " Порт закрыт!\r");
    }
}

//Запись в порт
void Port :: WriteToPort(QByteArray data)
{
    if(thisPort.isOpen())
    {
        thisPort.write(data);
        thisPort.waitForBytesWritten(TIMEOUT);
    }
}


//void Port :: ReadInPort()
//{
//    QByteArray data;

//    data.append(thisPort.readAll());
//    //((QString)(data.toInt())).toLatin1().toHex();
//    outPort(data);

//}


quint16 Port::Crc16(QByteArray pcBlock, quint16 len)
{
    unsigned short crc =0xFFFF;
    unsigned char i;
    //quint8 buf[25];
    unsigned char buf[len];

    for(int j = 0; j < len; j++)
    {
        buf[j] = (unsigned char)pcBlock[j];
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

void Port::tx_get_status()//запрос статуса
{
    QByteArray DataTxC(3,0);
    QByteArray DataTx(6,0);
    int i = 0;
    quint16 crc;
    DataTx[0] = SB;
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));
    DataTx[2] = DataTxC[1] = 0x00;
    DataTx[3] = DataTxC[2] = GET_STATUS;
    crc = Crc16(DataTxC, 3);
    DataTx[4] = (crc & 0xFF00)>>8;
    DataTx[5] = crc & 0x00FF;

    for(i = 0; i < 6; i++)
    {
        qDebug()<<DataTx[i];
    }
    //thisPort.write(DataTx);
    WriteToPort(DataTx);
    //thisPort.waitForReadyRead(/*TIMEOUT*/ 200);
}

void Port::tx_get_fh_param()//запрос текущих параметров гарнитуры
{
    QByteArray DataTxC(6,0);
    QByteArray DataTx(9,0);
    int i = 0;
    quint16 crc;
    DataTx[0] = SB;
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));
    DataTx[2] = DataTxC[1] = 0x03;
    DataTx[3] = DataTxC[2] = GET_FH_PARAM;
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
    //thisPort.write(DataTx);
    WriteToPort(DataTx);
    //thisPort.waitForReadyRead(/*TIMEOUT*/ 200);

}

void Port::tx_get_fh_key()//запрос ключа шифрования
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
    //thisPort.write(DataTx);

    WriteToPort(DataTx);
    //thisPort.waitForReadyRead(/*TIMEOUT*/ 200);
}

void Port::tx_rec_ok()
{
    QByteArray DataTxC(3,0);
    QByteArray DataTx(6,0);
    int i = 0;
    quint16 crc;
    DataTx[0] = SB;
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));
    DataTx[2] = DataTxC[1] = 0x00;
    DataTx[3] = DataTxC[2] = REC_OK;
    crc = Crc16(DataTxC, 3);
    DataTx[4] = (crc & 0xFF00)>>8;
    DataTx[5] = crc & 0x00FF;

    for(i = 0; i < 6; i++)
    {
        qDebug()<<DataTx[i];
    }
   // thisPort.write(DataTx);
    WriteToPort(DataTx);
}

////Запись в порт
//bool Port :: WriteToPort(QByteArray data)
//{
//    if(thisPort.isOpen())
//    {
//        thisPort.write(data);
//        if(thisPort.waitForBytesWritten(100))
//        {
//            if(thisPort.isDataTerminalReady())
//                return true;
//        }
//    }
//    else return false;
//}


uint8_t Port::ReadInPort()//Парсер
{
    QByteArray data_rx;
    while(thisPort.waitForReadyRead(TIMEOUT)){
        data_rx.append(thisPort.readAll());
    }
    unsigned short v_crc;
    unsigned char  hb_crc = 0xff,//crc пришедшей посылки (старший байт)
                   lb_crc = 0xff,//crc пришедшей посылки (младший байт)
                   len_dat, lb, hb,
                   dat_adr, adr;
//    if(data_rx.size() < 3)
//    {
//        return data_rx;
//    }

    QByteArray buf_rx;//[data_rx[2] + 3];
    QByteArray key_buf;//в этот массив пишется ответ на запрос GET_FH_KEY

    for(int i = 0; i < (data_rx[2] + 3); i++)
    {
        buf_rx[i] = data_rx[i+1];
    }

    v_crc   = Crc16(buf_rx, data_rx[2] + 3); //расчет crc пришедшей посылки
    lb_crc  = v_crc >> 8;  //                  //расчет crc пришедшей посылки (старший байт)
    hb_crc  = v_crc;         //                //расчет crc пришедшей посылки (младший байт)
    len_dat = data_rx[2] + 3;                //расчет длинны данных пришедшей посылки (старший байт)

    lb      = data_rx[len_dat + 2];          //crc пришедшей посылки (младший байт)
    hb      = data_rx[len_dat + 1];          //crc пришедшей посылки (старший байт)
    dat_adr = data_rx[1];                                      //адрес пришедшей посылки
    adr     = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF)))); //адрес, который должен быть
    if((lb == lb_crc) && (hb == hb_crc))                       //сравнение crc
    {
        if(dat_adr == adr)                                      //сравнение адреса
        {
            if(data_rx[3] == STATUS_OK)                         //анализ Ctrl
            {
                error_(("STATUS_OK")/*.toLocal8Bit()*/);        //вывод на консоль
                //qDebug()<<"STATUS_OK";
                //tx_rec_ok();
                return STATUS_OK;
            }
            else if(data_rx[3] == SEND_FH_PARAM)
            {
                //QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(data_rx);
                //MainWindow::Print(DataAsString);
                error_(("GET_PARAM")/*.toLocal8Bit()*/);
                qDebug()<<"GET_PARAM";
                //tx_rec_ok();
                return SEND_FH_PARAM;
            }
            else if(data_rx[3] == SEND_FH_KEY)
            {
                for(int i = 3, j = 0; i < (data_rx[2] + 3); i++ , j++)
                {
                    key_buf[j] = buf_rx[i];
                }
                error_("GET_FH_KEY");
                error_(key_buf.toHex().toUpper());
//                tx_rec_ok();
//                qDebug()<<"REC_ERROR";
                return SEND_FH_KEY;
            }
            else if(data_rx[3] == REC_ERROR)
            {
                error_("REC_ERROR");
                qDebug()<<"REC_ERROR";
                return REC_ERROR;
            }
        }
    }

    return BAD_PACKET;

    //data_rx.clear();
    //return 0;//Если все в плохо - отправляем "пустоту"
}
