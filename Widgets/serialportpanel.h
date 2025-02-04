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
    void sendData(const QByteArray &value);
    bool isSerialPortOpen();

signals:
    void onConnect();
    void onDisconnect();
    void onSendData(const QByteArray &data);
    void onSetParams(QSerialPortInfo *port_info, int baud_rate, QSerialPort::DataBits data_bits, QSerialPort::StopBits stop_bit, QSerialPort::Parity parity);

private slots:
    void ClickButton_refresh_serial_port();
    void ClickButton_connect_serial_port();
    void SelectSerialPort(int index);
    void onSerialClosed();

private:
    Ui::SerialPortPanel *ui;
    QList<QSerialPortInfo> portList;
    QSerialPortInfo *SerialPortInfo=NULL;
    QSerialPort SerialPort;

    SerialPortWorker *worker;
    QThread *serialThread;

    bool is_serial_port_open;
};

#endif // SERIALPORTPANEL_H
