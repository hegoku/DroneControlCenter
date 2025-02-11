#include "udppanel.h"
#include "ui_udppanel.h"
#include <QUdpSocket>
#include <QMessageBox>

UDPPanel::UDPPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UDPPanel)
{
    ui->setupUi(this);
    is_bind = 0;
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UDPPanel::handleUDPData, Qt::QueuedConnection);
    connect(udpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(udp_error_handler(QAbstractSocket::SocketError)));
    connect(ui->ConnectBtn, SIGNAL(clicked()), this, SLOT(connectUDP()));

    udp_thread = new UDPThread();
    connect(udp_thread, &UDPThread::onBeforeDisconnect, this, &UDPPanel::beforeDisconnect);
    // udp_thread->start();
}

UDPPanel::~UDPPanel()
{
    // udp_thread->quit();
    delete ui;
}

void UDPPanel::beforeDisconnect()
{
    emit onBeforeDisconnect();
}

void UDPPanel::connectUDP()
{
    quint16 port = ui->portEdit->text().toInt();
    if (port<=0 || port>65536) {
        QMessageBox::information(this, "Info", "Port must greater than 0 and less than or equal to 65536", QMessageBox::NoButton, QMessageBox::Close);
        return;
    }
    if (ui->ConnectBtn->text().compare("Connect")==0) {
        if (udp_thread->open(ui->ipEdit->text(), ui->portEdit->text().toInt())!=0) {
            QMessageBox::information(this, "Info", "Failed to connect to broadcast", QMessageBox::NoButton, QMessageBox::Close);
            return;
        }
        // if (!udpSocket->bind(QHostAddress::Any, port, QUdpSocket::ShareAddress)) {
        //     QMessageBox::information(this, "Info", "Failed to connect to broadcast", QMessageBox::NoButton, QMessageBox::Close);
        //     return;
        // }
        is_bind = 1;
        // udpSocket->connectToHost(QHostAddress(ui->ipEdit->text()), port, QIODevice::ReadWrite);
        ui->ipEdit->setEnabled(false);
        ui->portEdit->setEnabled(false);
        ui->ConnectBtn->setText("Disconnect");
        emit onConnect();
    } else {
        // emit onBeforeDisconnect();
        udp_thread->closeConnect();
        // udpSocket->close();
        is_bind = 0;
        ui->ipEdit->setEnabled(true);
        ui->portEdit->setEnabled(true);
        ui->ConnectBtn->setText("Connect");
        emit onDisconnect();
    }
}

void UDPPanel::handleUDPData()
{
    while (udpSocket->hasPendingDatagrams()) {
        QHostAddress hostAddress;
        quint16 hostPort;
        QByteArray data;
        data.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(data.data(), data.size(), &hostAddress, &hostPort);
        handleData(&data);
    }
}

void UDPPanel::udp_error_handler(QAbstractSocket::SocketError)
{

}

bool UDPPanel::isOpen()
{
    return udp_thread->isOpen();
    // return is_bind==1;
    // return udpSocket->isOpen();
}

void UDPPanel::sendData(const QByteArray &data)
{
    // if (udpSocket->isOpen()) {
    // if (is_bind==1) {
        // udpSocket->writeDatagram(data, QHostAddress(ui->ipEdit->text()), ui->portEdit->text().toInt());
        // udpSocket->write(data);
    // }
    udp_thread->sendData(data);
}
