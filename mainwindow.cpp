#include "mainwindow.h"
#include "Anotc/anotc_config_frame.h"
#include "ui_mainwindow.h"
#include "Anotc/anotc.h"
#include <qDebug>
#include <QScrollBar>
#include <QDateTime>
#include "DLog.h"

MainWindow* MainWindow::instance = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MainWindow::instance = this;
    ui->setupUi(this);

    DLog_print = MainWindow::printLog;

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
    connect(anotc_thread, &AnotcThread::onFlightDataComing, ui->dataTable, &DataTable::updateData);
    connect(anotc_thread, &AnotcThread::onFlightDataComing, ui->drone_3d_model, &DroneModel::onAttitudeUpdate);
    connect(anotc_thread, &AnotcThread::onFlightParamComing, ui->parameter_viewer, &ParameterForm::updateData);

    ui->serialPortWidget->setDataHandler(anotc_parse_data);
    ui->UDPWidget->handleData = anotc_parse_data;

    ui->dataTable->loadTable();
    anotc_thread->start(QThread::TimeCriticalPriority);
}

MainWindow::~MainWindow()
{
    anotc_thread->quit();
    timer->stop();
    delete ui;
}

void MainWindow::onSerialPortConnect()
{
    anotc_thread->setSendDelegate(MainWindow::sendData);
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

void MainWindow::showLog(struct anotc_blocking_queue_item item)
{
    if (item.frame.frame.fun==ANOTC_FRAME_LOG_STRING) {
        unsigned int color = item.frame.frame.data[0];
        int string_len = item.frame.frame.len - 1;
        DLogI(QString::fromLocal8Bit((char*)(item.frame.frame.data+1), string_len));
    }
}

void MainWindow::sendData(const QByteArray &data)
{
    if (MainWindow::instance->ui->SerialPortTab->isEnabled()) {
        MainWindow::instance->ui->serialPortWidget->sendData(data);
    } else {

    }
}

void MainWindow::printLog(QString content)
{
    MainWindow::instance->ui->logView->insertHtml(content);
    QScrollBar *scrollbar = MainWindow::instance->ui->logView->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}
