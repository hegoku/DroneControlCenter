#ifndef SERIALPORTWORKER_H
#define SERIALPORTWORKER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class SerialPortWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortWorker(QObject *parent = nullptr);
    void (*handleData)(QByteArray *data);

    void setParams(QSerialPortInfo *port_info, int baud_rate, QSerialPort::DataBits data_bits, QSerialPort::StopBits stop_bit, QSerialPort::Parity parity);
    bool open();
    bool isOpen();
    void close();

signals:
    void serialClosed();

public slots:
    void doDataReceiveWork();
    void SerialPortErrorHandler(QSerialPort::SerialPortError error);

private:
    QSerialPort *SerialPort;
};

#endif // SERIALPORTWORKER_H
