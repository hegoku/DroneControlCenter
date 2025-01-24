#ifndef SERIALPORTPANEL_H
#define SERIALPORTPANEL_H

#include <QWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QThread>
#include "serialportworker.h"

namespace Ui {
class SerialPortPanel;
}

class SerialPortPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPortPanel(QWidget *parent = nullptr);
    ~SerialPortPanel();

    void refreshSerialPort();
    void setDataHandler(void (*handleData)(QByteArray *data));

signals:
    void onConnect();
    void onDisconnect();

private slots:
    void ClickButton_refresh_serial_port();
    void ClickButton_connect_serial_port();
    void SelectSerialPort(int index);
    void SerialPortErrorHandler(QSerialPort::SerialPortError error);

private:
    Ui::SerialPortPanel *ui;
    QList<QSerialPortInfo> portList;
    QSerialPortInfo *SerialPortInfo=NULL;
    QSerialPort SerialPort;

    SerialPortWorker *worker;
    QThread serialThread;
};

#endif // SERIALPORTPANEL_H
