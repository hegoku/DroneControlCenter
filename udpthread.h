#ifndef UDPTHREAD_H
#define UDPTHREAD_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>

#ifdef _WIN64
#include <winsock2.h>
#include <windows.h>
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

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

private:
#ifdef _WIN64
    SOCKET socket_desc;
#else
    int socket_desc;
#endif
    struct sockaddr_in server_addr, client_addr;
    bool is_bind;
    char rx_buffer[2048];
};

#endif // UDPTHREAD_H
