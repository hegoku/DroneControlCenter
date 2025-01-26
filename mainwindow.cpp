#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Anotc/anotc.h"
#include <qDebug>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->serialPortWidget, SIGNAL(onConnect()), this, SLOT(onSerialPortConnect()));
    connect(ui->serialPortWidget, SIGNAL(onDisconnect()), this, SLOT(onSerialPortDisconnect()));
    connect(ui->UDPWidget, SIGNAL(onConnect()), this, SLOT(onUDPConnect()));
    connect(ui->UDPWidget, SIGNAL(onDisconnect()), this, SLOT(onUDPDisconnect()));

    connect(this, &MainWindow::onDataComing, this, &MainWindow::showLog);
    connect(this, &MainWindow::onDataComing, ui->frameTable, &FrameTable::updateData);

    timer = new QTimer();
    timer->setInterval(10);
    // timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(anotcTimerHanlder()));

    ui->serialPortWidget->setDataHandler(anotc_parse_data);
    ui->UDPWidget->handleData = anotc_parse_data;

    ui->frameTable->loadTable();
}

MainWindow::~MainWindow()
{
    timer->stop();
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

void MainWindow::anotcTimerHanlder()
{
    union _un_anotc_v8_frame data;
    while(anotc_queue.isEmpty()==false) {
        data = anotc_queue.first();
        emit onDataComing(&data);
        anotc_queue.removeFirst();
    }
    // timer->start();
}

void MainWindow::showLog(union _un_anotc_v8_frame *frame)
{
    if (frame->frame.fun==ANOTC_FRAME_LOG_STRING) {
        unsigned int color = frame->frame.data[0];
        int string_len = frame->frame.len - 1;
        ui->logView->insertPlainText(QString::fromLocal8Bit((char*)(frame->frame.data+1), string_len));
        QScrollBar *scrollbar = ui->logView->verticalScrollBar();
        if (scrollbar) {
            scrollbar->setSliderPosition(scrollbar->maximum());
        }
    }
}
