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

    anotc_status_label = new QLabel(this);
    anotc_status_label->setText("Received:0 Error:0 Exceed length:0");
    ui->statusbar->addPermanentWidget(anotc_status_label);

    connect(ui->serialPortWidget, SIGNAL(onConnect()), this, SLOT(onSerialPortConnect()));
    connect(ui->serialPortWidget, SIGNAL(onDisconnect()), this, SLOT(onSerialPortDisconnect()));
    connect(ui->UDPWidget, SIGNAL(onConnect()), this, SLOT(onUDPConnect()));
    connect(ui->UDPWidget, SIGNAL(onDisconnect()), this, SLOT(onUDPDisconnect()));

    timer = new QTimer();
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(anotcTimerHanlder()));

    anotc_thread = new AnotcThread();
    connect(anotc_thread, &AnotcThread::onFrameComing, this, &MainWindow::showLog);
    connect(anotc_thread, &AnotcThread::onFlightDataComing, ui->frameTable, &FrameTable::updateData);
    connect(anotc_thread, &AnotcThread::onFlightDataComing, ui->drone_3d_model, &DroneModel::onAttitudeUpdate);

    ui->serialPortWidget->setDataHandler(anotc_parse_data);
    ui->UDPWidget->handleData = anotc_parse_data;

    ui->frameTable->loadTable();
    anotc_thread->start();
}

MainWindow::~MainWindow()
{
    anotc_thread->quit();
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
    anotc_status_label->setText(QString("Received:%1 Error:%2 Exceed length:%3").arg(anotc_receive_count()).arg(anotc_receive_error_count()).arg( anotc_receive_exceed_count()));
}

void MainWindow::showLog(union _un_anotc_v8_frame frame)
{
    if (frame.frame.fun==ANOTC_FRAME_LOG_STRING) {
        unsigned int color = frame.frame.data[0];
        int string_len = frame.frame.len - 1;
        ui->logView->insertPlainText(QString::fromLocal8Bit((char*)(frame.frame.data+1), string_len));
        QScrollBar *scrollbar = ui->logView->verticalScrollBar();
        if (scrollbar) {
            scrollbar->setSliderPosition(scrollbar->maximum());
        }
    }
}
