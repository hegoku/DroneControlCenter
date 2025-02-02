#include "serialportworker.h"
#include <QThread>
#include <QByteArray>
#include "Anotc/anotc.h"

SerialPortWorker::SerialPortWorker(QObject *parent)
    : QObject{parent}
{
    SerialPort = new QSerialPort(this);
    // SerialPort->setReadBufferSize(1024*1024);
    handleData = anotc_parse_data;
    connect(SerialPort, &QSerialPort::readyRead, this, &SerialPortWorker::doDataReceiveWork);
    connect(SerialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(SerialPortErrorHandler(QSerialPort::SerialPortError)));
}

void SerialPortWorker::init()
{
    SerialPort = new QSerialPort();
    connect(SerialPort, &QSerialPort::readyRead, this, &SerialPortWorker::doDataReceiveWork);
    connect(SerialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(SerialPortErrorHandler(QSerialPort::SerialPortError)));
}

void SerialPortWorker::doDataReceiveWork()
{
    if (!SerialPort->isOpen()) return;
    if (SerialPort->bytesAvailable()==0) return;
    QByteArray raw = SerialPort->readAll();
    handleData(&raw);
}

void SerialPortWorker::setParams(QSerialPortInfo *port_info, int baud_rate, QSerialPort::DataBits data_bits, QSerialPort::StopBits stop_bit, QSerialPort::Parity parity)
{
    SerialPort->setPort(*port_info);
    SerialPort->setBaudRate(baud_rate);
    SerialPort->setDataBits(data_bits);
    SerialPort->setStopBits(stop_bit);
    SerialPort->setParity(parity);
}

bool SerialPortWorker::open()
{
    return SerialPort->open(QIODevice::ReadWrite);
}


bool SerialPortWorker::isOpen()
{
    return SerialPort->isOpen();
}

void SerialPortWorker::close()
{
    SerialPort->close();
}

void SerialPortWorker::SerialPortErrorHandler(QSerialPort::SerialPortError error)
{
    switch (error) {
    case QSerialPort::NoError:
        break;
    case QSerialPort::ResourceError:
        SerialPort->close();
        emit serialClosed();
        break;
    }
}

void SerialPortWorker::sendData(const QByteArray &data)
{
    if (SerialPort->isOpen()) {
        SerialPort->write(data);
    }
}
