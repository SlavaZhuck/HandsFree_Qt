#ifndef PORT_H
#define PORT_H

#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define SB             0xaa // стартовый байт
#define GET_STATUS     0x01 // запрос статуса устройства на линии

#define SEND_DATA      0x02 // передача массива данных
#define SEND_FH_PARAM  0x03 // передача текущих параметров гарнитуры
#define SEND_FH_KEY    0x04 // передача ключа шифрования
#define SEND_FH_CR_TP  0x05 // передача типа шифрования

#define STATUS_OK      0x10 // устройство работает в штатном режиме
#define STATUS_BAD     0x11 // устройство работает некорректно
#define REC_OK         0x12 // подтверждение безошибочного приёма данных
#define REC_ERROR      0x13 // в процессе приёма возникли ошибки
#define NO_COMAND      0x14 // принятой команды нет в списке
#define BAD_PACKET     0x15 // ошибка CRC


#define GET_FH_PARAM   0x23 // запрос текущих параметров гарнитуры
#define GET_FH_KEY     0x24 // запрос ключа шифрования
#define GET_FH_CR_TP   0x25 // запрос типа шифрования

#define ADR_PC         0x1
#define ADR_HF         0x3
#define ADR_TX(x)      x<<4
#define ADR_REC(x)     x

#define TIMEOUT         100

struct Settings
{
    QString                  name;
    qint32                   baudRate;
    QSerialPort::DataBits    dataBits;
    QSerialPort::Parity      parity;
    QSerialPort::StopBits    stopBits;
    QSerialPort::FlowControl flowControl;
};

class Port : public QObject
{
    Q_OBJECT

public:

    explicit Port(QObject *parent = 0);

    ~Port();

    QSerialPort thisPort;

    Settings    SettingsPort;

signals:
    void finished_Port();

    void error_(QString err);

    void outPort(QString data);

    void sendParam();

    void timerStartSignal();

    void timerStop();

public slots:
    void DisconnectPort();

    void ConnectPort(void);

    void Write_Settings_Port(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);

    void process_Port();

    void WriteToPort(QByteArray data);

    uint8_t ReadInPort();

    quint16 Crc16(QByteArray pcBlock, quint16 len);

    QByteArray ParamsGet();

    QByteArray BatterGet();

    void tx_get_status();

    void tx_get_fh_param();

    void tx_get_fh_key();

    void rx_rec_ok();

private slots:

    void handleError(QSerialPort::SerialPortError error);

public:

};
#endif // PORT_H
