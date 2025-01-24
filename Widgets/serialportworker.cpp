#include "serialportworker.h"
#include <QThread>
#include <QByteArray>
#include "Anotc/anotc.h"

SerialPortWorker::SerialPortWorker(QSerialPort *ser, QObject *parent)
    : QObject{parent}, SerialPort(ser)
{
    handleData = anotc_parse_data;
}

void SerialPortWorker::doDataReceiveWork()
{
    QByteArray raw = SerialPort->readAll();
    handleData(&raw);
}
