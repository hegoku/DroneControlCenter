#ifndef SERIALPORTWORKER_H
#define SERIALPORTWORKER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class SerialPortWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortWorker(QSerialPort *ser, QObject *parent = nullptr);
    void (*handleData)(QByteArray *data);

signals:
    void sendResultToGUI(QByteArray raw);

public slots:
    void doDataReceiveWork();

private:
    QSerialPort *SerialPort;
};

#endif // SERIALPORTWORKER_H
