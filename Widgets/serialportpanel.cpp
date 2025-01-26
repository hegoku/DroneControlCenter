#include "serialportpanel.h"
#include "ui_serialportpanel.h"
#include <QMessageBox>
#include "serialportworker.h"
#include <Qt>

SerialPortPanel::SerialPortPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialPortPanel)
{
    ui->setupUi(this);
    connect(ui->serialPortConnectButton, SIGNAL(clicked()), this, SLOT(ClickButton_connect_serial_port()));
    connect(ui->refreshSerialPortButton, SIGNAL(clicked()), this, SLOT(ClickButton_refresh_serial_port()));

    refreshSerialPort();
    // connect(&SerialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(SerialPortErrorHandler(QSerialPort::SerialPortError)));

    worker = new SerialPortWorker();
    worker->moveToThread(&serialThread);
    connect(worker, &SerialPortWorker::serialClosed, this, &SerialPortPanel::onSerialClosed);
    serialThread.start();

    // connect(&serialThread, &QThread::finished, worker, &QObject::deleteLater);
    // connect(&SerialPort, &QSerialPort::readyRead, worker, &SerialPortWorker::doDataReceiveWork);
}

SerialPortPanel::~SerialPortPanel()
{
    serialThread.quit();
    serialThread.wait();
    // SerialPort.close();
    delete ui;
}

void SerialPortPanel::refreshSerialPort()
{
    disconnect(ui->serialPortComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SelectSerialPort(int)));
    portList = QSerialPortInfo::availablePorts();
    ui->serialPortComboBox->clear();
    connect(ui->serialPortComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SelectSerialPort(int)));
    for (int i=0;i<portList.size();i++) {
        ui->serialPortComboBox->addItem(portList[i].portName());
    }
}

void SerialPortPanel::ClickButton_refresh_serial_port()
{
    refreshSerialPort();
}

void SerialPortPanel::SelectSerialPort(int index)
{
    ui->serialPortConnectButton->setEnabled(true);
    ui->refreshSerialPortButton->setEnabled(true);
    SerialPortInfo = &portList[index];
}

void SerialPortPanel::ClickButton_connect_serial_port()
{
    QSerialPort::DataBits data_bits = static_cast<QSerialPort::DataBits>(ui->DataBitsSelection->currentText().toInt());
    QSerialPort::StopBits stop_bit = QSerialPort::OneStop;
    if (ui->StopBitSelection->currentText().compare("1.5")==0) {
        stop_bit = QSerialPort::OneAndHalfStop;
    } else if (ui->StopBitSelection->currentText().compare("2")==0) {
        stop_bit = QSerialPort::TwoStop;
    }

    QSerialPort::Parity parity = QSerialPort::NoParity;
    if (ui->ParitySelection->currentText().compare("Even")==0) {
        parity = QSerialPort::EvenParity;
    } else if (ui->ParitySelection->currentText().compare("Odd")==0) {
        parity = QSerialPort::OddParity;
    }

    if (ui->serialPortConnectButton->text().compare(QString("Connect"))==0) {
        QMetaObject::invokeMethod(worker, [=]{
            if (worker->isOpen()) {
                QMetaObject::invokeMethod(this, [=]{
                    QMessageBox::information(this, "Info", "The Serial Port has been opened.", QMessageBox::NoButton, QMessageBox::Close);
                    return;
                }, Qt::QueuedConnection);
            } else {
                worker->setParams(SerialPortInfo, ui->lineEditBaudRate->text().toInt(), data_bits, stop_bit, parity);
                if (!worker->open()) {
                    QMetaObject::invokeMethod(this, [=]{
                        QMessageBox::information(this, "Info", "Open Serial Port failed.", QMessageBox::NoButton, QMessageBox::Close);
                        return;
                    }, Qt::QueuedConnection);
                } else {
                    QMetaObject::invokeMethod(this, [=]{
                        ui->serialPortComboBox->setEnabled(false);
                        ui->refreshSerialPortButton->setEnabled(false);
                        ui->lineEditBaudRate->setEnabled(false);
                        ui->DataBitsSelection->setEnabled(false);
                        ui->ParitySelection->setEnabled(false);
                        ui->StopBitSelection->setEnabled(false);
                        ui->serialPortConnectButton->setText("Disconnect");
                        emit onConnect();
                    }, Qt::QueuedConnection);
                }
            }
        }, Qt::QueuedConnection);
        // if (SerialPort.isOpen()) {
        //     QMessageBox::information(this, "Info", "The Serial Port has been opened.", QMessageBox::NoButton, QMessageBox::Close);
        //     return;
        // }
        // SerialPort.setPort(*SerialPortInfo);
        // SerialPort.setBaudRate(ui->lineEditBaudRate->text().toInt());
        // SerialPort.setDataBits(data_bits);
        // SerialPort.setStopBits(stop_bit);
        // SerialPort.setParity(parity);
        // serialThread.start();
        // if (!SerialPort.open(QIODevice::ReadWrite)) {
        //     QMessageBox::information(this, "Info", "Open Serial Port failed.", QMessageBox::NoButton, QMessageBox::Close);
        //     return;
        // }
        // ui->serialPortComboBox->setEnabled(false);
        // ui->refreshSerialPortButton->setEnabled(false);
        // ui->lineEditBaudRate->setEnabled(false);
        // ui->DataBitsSelection->setEnabled(false);
        // ui->ParitySelection->setEnabled(false);
        // ui->StopBitSelection->setEnabled(false);
        // ui->serialPortConnectButton->setText("Disconnect");
        // emit onConnect();
    } else {
        ui->serialPortComboBox->setEnabled(true);
        ui->refreshSerialPortButton->setEnabled(true);
        ui->lineEditBaudRate->setEnabled(true);
        ui->DataBitsSelection->setEnabled(true);
        ui->ParitySelection->setEnabled(true);
        ui->StopBitSelection->setEnabled(true);
        ui->serialPortConnectButton->setText("Connect");
        QMetaObject::invokeMethod(worker, [=]{
            if (!worker->isOpen()) {
                QMetaObject::invokeMethod(this, [=]{
                    QMessageBox::information(this, "Info", "The Serial Port is closed", QMessageBox::NoButton, QMessageBox::Close);
                    return;
                }, Qt::QueuedConnection);
            } else {
                worker->close();
                QMetaObject::invokeMethod(this, [=]{
                    emit onDisconnect();
                }, Qt::QueuedConnection);
            }
        }, Qt::QueuedConnection);
        // if (!SerialPort.isOpen()) {
        //     QMessageBox::information(this, "Info", "The Serial Port is closed", QMessageBox::NoButton, QMessageBox::Close);
        //     return;
        // }
        // SerialPort.clear();
        // serialThread.quit();
        // SerialPort.close();
        // emit onDisconnect();
    }
}

void SerialPortPanel::setDataHandler(void (*handleData)(QByteArray *data))
{
    worker->handleData = handleData;
}

// void SerialPortPanel::SerialPortErrorHandler(QSerialPort::SerialPortError error)
// {
//     switch (error) {
//     case QSerialPort::NoError:
//         break;
//     case QSerialPort::ResourceError:
//         serialThread.quit();
//         SerialPort.close();
//         ui->serialPortComboBox->setEnabled(true);
//         ui->refreshSerialPortButton->setEnabled(true);
//         ui->lineEditBaudRate->setEnabled(true);
//         ui->DataBitsSelection->setEnabled(true);
//         ui->ParitySelection->setEnabled(true);
//         ui->StopBitSelection->setEnabled(true);
//         ui->serialPortConnectButton->setText("Connect");
//         emit onDisconnect();
//         break;
//     }
// }

void SerialPortPanel::onSerialClosed()
{
    ui->serialPortComboBox->setEnabled(true);
    ui->refreshSerialPortButton->setEnabled(true);
    ui->lineEditBaudRate->setEnabled(true);
    ui->DataBitsSelection->setEnabled(true);
    ui->ParitySelection->setEnabled(true);
    ui->StopBitSelection->setEnabled(true);
    ui->serialPortConnectButton->setText("Connect");
}
