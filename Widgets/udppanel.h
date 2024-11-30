#ifndef UDPPANEL_H
#define UDPPANEL_H

#include <QWidget>
#include <QUdpSocket>

namespace Ui {
class UDPPanel;
}

class UDPPanel : public QWidget
{
    Q_OBJECT

public:
    explicit UDPPanel(QWidget *parent = nullptr);
    ~UDPPanel();

signals:
    void onConnect();
    void onDisconnect();

private slots:
    void connectUDP();
    void handleUDPData();
    void udp_error_handler(QAbstractSocket::SocketError);

private:
    Ui::UDPPanel *ui;
    QUdpSocket *udpSocket;
};

#endif // UDPPANEL_H
