#include "port.h"
#include <qdebug.h>
#include <QString>
#include <QTextCodec>
#include <QThread>


Port :: Port(QObject *parent) : QObject(parent)
{

}

Port :: ~Port()
{
    qDebug("Всем спасибо и до свидания");//Виден в дебаге при отключении
    emit finished_Port();
}

void Port :: process_Port()
{
    qDebug("Здаров");//виден в дебаге при подключении
}

//Запись насроек----------------------------------------------------------------------------------
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

//Подключение порта------------------------------------------------------------------------------
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
            {
                error_((SettingsPort.name+ "  Порт открыт:\r"));//если открыт - прнимаем надпись "Порт открыт"
                uint8_t status = 0;
                int k = 0;

                while((status != STATUS_OK) && (k < 10))//Идет запрос статуса пока не вернется STATUS_OK
                {
                    QThread::sleep(1);
                    tx_get_status();//Запрос статуса
                    status = ReadInPort();
                    k++;
                }
                if(k >= 9)
                {
                    error_("Статус устройства не подтвержден\nв положенное время\r");
                    DisconnectPort();
                }
                else
                {
                    tx_get_fh_param();//Запрос параметров
                    ReadInPort();
                    tx_get_fh_key();//Запрос ключа шифрования записанного на гарнитуре
                    ReadInPort();
                    timerStartSignal();
                }

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

//Выдает сообщение об ошибке---------------------------------------------------------------------
void Port::handleError(QSerialPort::SerialPortError error)
{
    if ( (thisPort.isOpen()) && (error == QSerialPort::ResourceError))
    {
        error_(thisPort.errorString().toLocal8Bit());
        DisconnectPort();
    }
}

//Отключение порта-------------------------------------------------------------------------------
void  Port::DisconnectPort()
{
    if(thisPort.isOpen())
    {
        timerStop();
        thisPort.close();        
        error_(SettingsPort.name.toLocal8Bit() + " Порт закрыт!\r");

    }
}

//Запись данных в порт-----------------------------------------------------------------------------
void Port :: WriteToPort(QByteArray data)
{
    if(thisPort.isOpen())
    {
        thisPort.write(data);
        thisPort.waitForBytesWritten(TIMEOUT);
    }
}

//Функция расчета CRC------------------------------------------------------------------------------
quint16 Port::Crc16(QByteArray pcBlock, quint16 len)
{
    unsigned short crc =0xFFFF;
    unsigned char i;
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

//Запрос статуса устройства-----------------------------------------------------------------------
void Port::tx_get_status()
{
    QByteArray DataTxC(3,0);//Массив для расчета CRC
    QByteArray DataTx(6,0); //Массив данных
    quint16 crc;

    DataTx[0] = SB;         //Стартовый байт
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));//Адрес
    DataTx[2] = DataTxC[1] = 0x00;      //Длина данных
    DataTx[3] = DataTxC[2] = GET_STATUS;//Ctrl
    crc = Crc16(DataTxC, 3);            //расчет crc отправляемой посылки
    DataTx[4] = (crc & 0xFF00)>>8;      //расчет crc отправляемой посылки (старший байт)
    DataTx[5] = crc & 0x00FF;           //расчет crc отправляемой посылки (дладший байт)

    qDebug()<<DataTx.toHex().toUpper(); //Отображение в дебагере
    WriteToPort(DataTx);                //Запись в порт
}

//Запрос текущих параметров гарнитуры---------------------------------------------------------------
void Port::tx_get_fh_param()
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

    qDebug()<<DataTx.toHex().toUpper();//Отображение в дебагере
    WriteToPort(DataTx);               //Запись в порт
}

//Запрос ключа шифрования--------------------------------------------------------------------------
void Port::tx_get_fh_key()
{
    QByteArray DataTxC(6,0);//Массив для расчета CRC
    QByteArray DataTx(9,0); //Массив данных
    quint16 crc;

    DataTx[0] = SB;         //Стартовый байт
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));//Адрес
    DataTx[2] = DataTxC[1] = 0x03;      //Длина данных
    DataTx[3] = DataTxC[2] = GET_FH_KEY;//Ctrl
    DataTx[4] = DataTxC[3] = 0x01;
    DataTx[5] = DataTxC[4] = 0x02;
    DataTx[6] = DataTxC[5] = 0x03;
    crc = Crc16(DataTxC, 6);      //расчет crc отправляемой посылки
    DataTx[7] = (crc & 0xFF00)>>8;//расчет crc отправляемой посылки (старший байт)
    DataTx[8] = crc & 0x00FF;     //расчет crc отправляемой посылки (дладший байт)

    qDebug()<<DataTx.toHex().toUpper();//Отображение в дебагере
    WriteToPort(DataTx);               //Запись в порт
}

//Подтверждение безошибочного приема--------------------------------------------------------------
void Port::rx_rec_ok()
{
    QByteArray DataTxC(3,0);//Массив для расчета CRC
    QByteArray DataTx(6,0);//Массив данных
    quint16 crc;

    DataTx[0] = SB;//Стартовый байт
    DataTx[1] = DataTxC[0] = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF))));//Адрес
    DataTx[2] = DataTxC[1] = 0x00;  //Длина данных
    DataTx[3] = DataTxC[2] = REC_OK;//Ctrl
    crc = Crc16(DataTxC, 3);        //расчет crc принятой посылки
    DataTx[4] = (crc & 0xFF00)>>8;  //расчет crc принимаемой посылки (старший байт)
    DataTx[5] = crc & 0x00FF;       //расчет crc принимаемой посылки (дладший байт)

    qDebug()<<DataTx.toHex().toUpper();//Отображение в дебагере
    WriteToPort(DataTx);               //Запись в порт
}

QByteArray buf_param;//Массив МАС адреса
QByteArray buf_batter;//Массив заряда батареи
//Чтение в порт-------------------------------------------------------------------------------------
uint8_t Port::ReadInPort()//Парсер
{
    QByteArray data_rx;                      //Массив принятых данных
    while(thisPort.waitForReadyRead(TIMEOUT))//Ожидание считывания посылки до конца
    {
        data_rx.append(thisPort.readAll());  //Чтение посылки до конца
    }

    unsigned short v_crc;          //Принятый CRC
    unsigned char  hb_crc = 0xff,  //crc пришедшей посылки (старший байт)
                   lb_crc = 0xff,  //crc пришедшей посылки (младший байт)
                   len_dat, lb, hb,//Длина данных, старший байт CRC, младший байт CRC
                   dat_adr, adr;

    QByteArray buf_rx;//[data_rx[2] + 3];
    QByteArray key_buf;//в этот массив пишется ответ на запрос GET_FH_KEY

    for(int i = 0; i < (data_rx[2] + 3); i++)
    {
        buf_rx[i] = data_rx[i+1];
    }

    v_crc   = Crc16(buf_rx, data_rx[2] + 3); //расчет crc пришедшей посылки
    lb_crc  = v_crc >> 8;                    //расчет crc пришедшей посылки (старший байт)
    hb_crc  = v_crc;                         //расчет crc пришедшей посылки (младший байт)
    len_dat = data_rx[2] + 3;                //расчет длинны данных пришедшей посылки (старший байт)
    lb      = data_rx[len_dat + 2];          //crc пришедшей посылки (младший байт)
    hb      = data_rx[len_dat + 1];          //crc пришедшей посылки (старший байт)
    dat_adr = data_rx[1];                                      //адрес пришедшей посылки
    adr     = (0xff & ((ADR_TX(ADR_PC)) | (ADR_REC(ADR_HF)))); //адрес, который должен быть
    if((lb == lb_crc) && (hb == hb_crc))                       //сравнение crc
    {
        if(dat_adr == adr)                                     //сравнение адреса
        {
            if(data_rx[3] == STATUS_OK)//Ответ на запрос статуса
            {
                error_(("STATUS_OK")); //вывод на консоль
                qDebug()<<"STATUS_OK";
                return STATUS_OK;
            }
            else if(data_rx[3] == SEND_FH_PARAM)//Ответ на запрос параметров
            {
                for(int i = 0, j = 10; j < 12; i++, j++)
                {
                    buf_batter[i] = data_rx[j];
                }

                QByteArray compare_arr;

                for(int i = 0, j = 4; j < 10; i++, j++)
                {
                    compare_arr[i] = data_rx[j];
                }
                if(compare_arr != buf_param)
                {
                    for(int i = 0, j = 4; j < 10; i++, j++)
                    {
                        buf_param[i] = data_rx[j];
                    }

                    QString str(buf_param.toHex().toUpper());
                    for(int i = 0; i < str.length(); i = i + 3)
                        str =str.insert(i, ":");  //вставляем ":" в нужные маста
                    str = str.remove(0, 1);       //удаляем нулевой пробел
                    error_(("GET_PARAM"));        //вывод на консоль
                    error_(("MAC адрес: " + str));//вывод на консоль
                    qDebug()<<"GET_PARAM";
                    qDebug()<<("MAC адрес: " + str);
                    ParamsGet();
                    sendParam();
                }
                qDebug()<<data_rx.toHex().toUpper();
                return SEND_FH_PARAM;
            }
            else if(data_rx[3] == SEND_FH_KEY)//Ответ на запрос ключа шифрования
            {
                for(int i = 3, j = 0; i < (data_rx[2] + 3); i++ , j++)
                {
                    key_buf[j] = buf_rx[i];
                }
                error_("Ключ:");                  //вывод на консоль
                error_(key_buf.toHex().toUpper());//вывод на консоль записанного ключа
                return SEND_FH_KEY;
            }
            else if(data_rx[3] == REC_OK)//
            {
                error_("Ключ загружен\r");
                qDebug()<<"REC_OK";
                return REC_OK;
            }
            else if(data_rx[3] == REC_ERROR)
            {
                error_("REC_ERROR\r");
                qDebug()<<"REC_ERROR";
                return REC_ERROR;
            }
            else if(data_rx[3] == BAD_PACKET)
            {
                error_("Ошибка CRC\r");
                qDebug()<<"CRC_ERROR";
                return REC_ERROR;
            }
        }
    }

    return BAD_PACKET;
}

//Отсылаем значение buf_param в MainWindow
QByteArray Port::ParamsGet()
{
    QByteArray para = buf_param;

    return para;
}

//Отсылаем значение buf_batter в MainWindow
QByteArray Port::BatterGet()
{
    QByteArray batter = buf_batter;

    return  batter;
}
