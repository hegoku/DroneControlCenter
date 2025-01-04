#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Anotc/anotc.h"
#include <qDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->serialPortWidget, SIGNAL(onConnect()), this, SLOT(onSerialPortConnect()));
    connect(ui->serialPortWidget, SIGNAL(onDisconnect()), this, SLOT(onSerialPortDisconnect()));
    connect(ui->UDPWidget, SIGNAL(onConnect()), this, SLOT(onUDPConnect()));
    connect(ui->UDPWidget, SIGNAL(onDisconnect()), this, SLOT(onUDPDisconnect()));

    timer = new QTimer();
    timer->setInterval(10);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(anotcTimerHanlder()));

    anotc_set_hander(anotc_handler);
    ui->serialPortWidget->handleData = anotc_parse_data;
    ui->UDPWidget->handleData = anotc_parse_data;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSerialPortConnect()
{
    ui->UDPTab->setEnabled(false);
    timer->start();
}

void MainWindow::onSerialPortDisconnect()
{
    timer->stop();
    anotc_reset();
    ui->UDPTab->setEnabled(true);
}

void MainWindow::onUDPConnect()
{
    ui->SerialPortTab->setEnabled(false);
    timer->start();
}

void MainWindow::onUDPDisconnect()
{
    timer->stop();
    anotc_reset();
    ui->SerialPortTab->setEnabled(true);
}

QList<union _un_anotc_v8_frame> MainWindow::anotc_queue = QList<union _un_anotc_v8_frame>();

void MainWindow::anotc_handler(union _un_anotc_v8_frame *frame)
{
    MainWindow::anotc_queue.append(*frame);
}

void MainWindow::anotcTimerHanlder()
{
    union _un_anotc_v8_frame data;
    while(MainWindow::anotc_queue.isEmpty()==false) {
        data = MainWindow::anotc_queue.first();
        MainWindow::anotc_queue.removeFirst();
    }
    timer->start();
}
