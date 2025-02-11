#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include <QObject>
#include <QThread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <QByteArray>
#include <QMutex>

class UDPThread : public QThread
{
    Q_OBJECT
public:
    explicit UDPThread(QObject *parent = nullptr);
    int open(QString ip, unsigned short port);
    bool isOpen();
    void run();
    void closeConnect();
    void (*handleData)(QByteArray *data);
    void sendData(const QByteArray &data);

signals:
    void onConnect();
    void onBeforeDisconnect();
    void onDisconnect();

private:
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    bool is_bind;
    char rx_buffer[2048];

    static QMutex mutex;
};

#endif // UDPTHREAD_H
