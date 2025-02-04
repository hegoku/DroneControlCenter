#include "udppanel.h"
#include "ui_udppanel.h"
#include <QUdpSocket>
#include <QMessageBox>

UDPPanel::UDPPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UDPPanel)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UDPPanel::handleUDPData);
    connect(udpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(udp_error_handler(QAbstractSocket::SocketError)));
    connect(ui->ConnectBtn, SIGNAL(clicked()), this, SLOT(connectUDP()));
}

UDPPanel::~UDPPanel()
{
    delete ui;
}

void UDPPanel::connectUDP()
{
    quint16 port = ui->portEdit->text().toInt();
    if (port<=0 || port>65536) {
        QMessageBox::information(this, "Info", "Port must greater than 0 and less than or equal to 65536", QMessageBox::NoButton, QMessageBox::Close);
        return;
    }
    if (ui->ConnectBtn->text().compare("Connect")==0) {
        if (!udpSocket->bind(QHostAddress::Any, port, QUdpSocket::ShareAddress)) {
            QMessageBox::information(this, "Info", "Failed to connect to broadcast", QMessageBox::NoButton, QMessageBox::Close);
            return;
        }
        ui->ipEdit->setEnabled(false);
        ui->portEdit->setEnabled(false);
        ui->ConnectBtn->setText("Disconnect");
        emit onConnect();
    } else {
        udpSocket->close();
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
    return udpSocket->isOpen();
}

void UDPPanel::sendData(const QByteArray &data)
{
    if (udpSocket->isOpen()) {
        udpSocket->write(data);
    }
}
