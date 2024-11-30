#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->serialPortWidget, SIGNAL(onConnect()), this, SLOT(onSerialPortConnect()));
    connect(ui->serialPortWidget, SIGNAL(onDisconnect()), this, SLOT(onSerialPortDisconnect()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSerialPortConnect()
{
    ui->UDPTab->setEnabled(false);
}

void MainWindow::onSerialPortDisconnect()
{
    ui->UDPTab->setEnabled(true);
}
