#ifndef SERIALPORTPANEL_H
#define SERIALPORTPANEL_H

#include <QWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

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

signals:
    void onConnect();
    void onDisconnect();

private slots:
    void ClickButton_refresh_serial_port();
    void ClickButton_connect_serial_port();
    void SelectSerialPort(int index);
    void SerialPortErrorHandler(QSerialPort::SerialPortError error);
    void SerialRecvHandler();

private:
    Ui::SerialPortPanel *ui;
    QList<QSerialPortInfo> portList;
    QSerialPortInfo *SerialPortInfo=NULL;
    QSerialPort SerialPort;
};

#endif // SERIALPORTPANEL_H
