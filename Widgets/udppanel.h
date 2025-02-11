#ifndef UDPPANEL_H
#define UDPPANEL_H

#include <QWidget>
#include <QUdpSocket>
#include "udpthread.h"

namespace Ui {
class UDPPanel;
}

class UDPPanel : public QWidget
{
    Q_OBJECT

public:
    explicit UDPPanel(QWidget *parent = nullptr);
    ~UDPPanel();

    void (*handleData)(QByteArray *data);
    bool isOpen();
    void sendData(const QByteArray &data);

public slots:
    void beforeDisconnect();

signals:
    void onConnect();
    void onBeforeDisconnect();
    void onDisconnect();

private slots:
    void connectUDP();
    void handleUDPData();
    void udp_error_handler(QAbstractSocket::SocketError);

private:
    Ui::UDPPanel *ui;
    QUdpSocket *udpSocket;
    int is_bind;

    UDPThread *udp_thread;
};

#endif // UDPPANEL_H
