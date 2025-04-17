#include "udpthread.h"
#include <unistd.h>
#include "Anotc/anotc.h"

UDPThread::UDPThread(QObject *parent)
    : QThread{parent}
{
    handleData = anotc_parse_data;
    is_bind = false;
}


int UDPThread::open(QString ip, unsigned short port)
{
    if (isOpen()) return -1;
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if(socket_desc < 0){
        qDebug("Error while creating socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    QByteArray ip_d = ip.toLocal8Bit();
    client_addr.sin_addr.s_addr = inet_addr(ip_d.data());

    int opt=-1;
    int nb = setsockopt(socket_desc, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if(nb==-1) {
        qDebug("set socket error...");
        return -1;
    }
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0){
        qDebug("Couldn't bind to the port");
        return -1;
    }
    is_bind = true;
    this->start();

    return 0;
}

bool UDPThread::isOpen()
{
    return is_bind;
}

void UDPThread::closeConnect()
{
    is_bind = false;
    this->quit();
#ifdef _WIN64
    closesocket(socket_desc);
#else
    close(socket_desc);
#endif
}

void UDPThread::run()
{
    int len = 0;
    struct sockaddr_in c_addr;
    socklen_t client_struct_length = sizeof(struct sockaddr_in);
    for(;;) {
        len = recvfrom(socket_desc, rx_buffer, sizeof(rx_buffer), 0, (struct sockaddr*)&c_addr, &client_struct_length);
        if (len>0) {
            QByteArray data = QByteArray::fromRawData(rx_buffer, len);
            handleData(&data);
        }
    }
}

void UDPThread::sendData(const QByteArray &data)
{
    if (!is_bind) {
        return;
    }
    int client_struct_length = sizeof(struct sockaddr_in);

    sendto(socket_desc, data.data(), data.length(), 0, (struct sockaddr*)&client_addr, client_struct_length);
}
