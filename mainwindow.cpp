#include "mainwindow.h"
#include "Anotc/anotc_config_frame.h"
#include "ui_mainwindow.h"
#include "Anotc/anotc.h"
#include <qDebug>
#include <QScrollBar>
#include <QDateTime>
#include "DLog.h"
#include "Anotc/anotc_custom_frame.h"

MainWindow* MainWindow::instance = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MainWindow::instance = this;

    ui->setupUi(this);

    DLog_print = MainWindow::printLog;

    cpu_load_label = new QLabel(this);
    cpu_load_label->setText("CPU: 0%");
    ui->statusbar->addWidget(cpu_load_label);

    imu_status_label = new QLabel(this);
    imu_status_label->setText("IMU:OFF");
    ui->statusbar->addWidget(imu_status_label);

    compass_status_label = new QLabel(this);
    compass_status_label->setText("COMPASS:OFF");
    ui->statusbar->addWidget(compass_status_label);

    baro_status_label = new QLabel(this);
    baro_status_label->setText("BARO:OFF");
    ui->statusbar->addWidget(baro_status_label);

    anotc_status_label = new QLabel(this);
    anotc_status_label->setText("Received:0 Error:0 Exceed length:0");
    ui->statusbar->addPermanentWidget(anotc_status_label);

    connect(ui->serialPortWidget, SIGNAL(onConnect()), this, SLOT(onSerialPortConnect()));
    connect(ui->serialPortWidget, SIGNAL(onDisconnect()), this, SLOT(onSerialPortDisconnect()));
    connect(ui->serialPortWidget, &SerialPortPanel::onBeforeDisconnect, this, &MainWindow::onBeforeDisconnect);

    connect(ui->UDPWidget, SIGNAL(onConnect()), this, SLOT(onUDPConnect()));
    connect(ui->UDPWidget, SIGNAL(onDisconnect()), this, SLOT(onUDPDisconnect()));
    connect(ui->UDPWidget, &UDPPanel::onBeforeDisconnect, this, &MainWindow::onBeforeDisconnect);

    connect(this, &MainWindow::onConnect, ui->parameter_viewer, &ParameterForm::onConnect);
    connect(this, &MainWindow::onDisconnect, ui->parameter_viewer, &ParameterForm::onDisconnect);
    connect(this, &MainWindow::onConnect, ui->mixer_form, &MixerForm::onConnect);
    connect(this, &MainWindow::onDisconnect, ui->mixer_form, &MixerForm::onDisconnect);

    timer = new QTimer();
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(anotcTimerHanlder()));

    anotc_thread = new AnotcThread();
    anotc_thread->setSendDelegate(MainWindow::sendData);
    connect(anotc_thread, &AnotcThread::onFrameComing, this, &MainWindow::showLog);
    connect(anotc_thread, &AnotcThread::onCheckFrameComing, ui->parameter_viewer, &ParameterForm::receiveCheckFrame);
    connect(anotc_thread, &AnotcThread::onCheckFrameComing, ui->calibration_form, &CalibrationForm::receiveCheckFrame);

    connect(anotc_thread, &AnotcThread::onFlightDataComing, ui->dataTable, &DataTable::updateData);
    connect(anotc_thread, &AnotcThread::onFlightDataComing, ui->drone_3d_model, &DroneModel::onAttitudeUpdate);
    connect(anotc_thread, &AnotcThread::onFlightDataComing, this, &MainWindow::flightData);
    connect(anotc_thread, &AnotcThread::onFlightDataComing, ui->calibration_form, &CalibrationForm::updateData);
    connect(anotc_thread, &AnotcThread::onFlightDataComing, ui->mixer_form, &MixerForm::onFlightUpdate);

    connect(anotc_thread, &AnotcThread::onFlightParamComing, ui->parameter_viewer, &ParameterForm::updateData);
    connect(anotc_thread, &AnotcThread::onFlightParamComing, this, &MainWindow::getDeviceInfo);

    connect(anotc_thread, &AnotcThread::onCMDResponseComing, ui->calibration_form, &CalibrationForm::cmdResponse);

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
    ui->UDPTab->setEnabled(false);
    emit onConnect();
    anotc_send_custom_connect();
    timer->start();
}

void MainWindow::onSerialPortDisconnect()
{
    emit onDisconnect();
    timer->stop();
    anotc_reset();
    ui->UDPTab->setEnabled(true);
    imu_status_label->setText("IMU:OFF");
    compass_status_label->setText("COMPASS:OFF");
    baro_status_label->setText("BARO:OFF");
}

void MainWindow::onUDPConnect()
{
    ui->SerialPortTab->setEnabled(false);
    emit onConnect();
    anotc_send_custom_connect();
    timer->start();
}

void MainWindow::onUDPDisconnect()
{
    emit onDisconnect();
    timer->stop();
    anotc_reset();
    ui->SerialPortTab->setEnabled(true);
    imu_status_label->setText("IMU:OFF");
    compass_status_label->setText("COMPASS:OFF");
    baro_status_label->setText("BARO:OFF");
}

void MainWindow::onBeforeDisconnect()
{
    anotc_send_custom_disconnect();
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
    if (MainWindow::instance->ui->serialPortWidget->isSerialPortOpen()) {
        MainWindow::instance->ui->serialPortWidget->sendData(data);
    } else if (MainWindow::instance->ui->UDPWidget->isOpen()) {
        MainWindow::instance->ui->UDPWidget->sendData(data);
    }
}

void MainWindow::printLog(QString content)
{
    QTextCursor cursor = MainWindow::instance->ui->logView->textCursor();
    cursor.movePosition(QTextCursor::End);
    MainWindow::instance->ui->logView->setTextCursor(cursor);
    MainWindow::instance->ui->logView->insertHtml(content);
    QScrollBar *scrollbar = MainWindow::instance->ui->logView->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

void MainWindow::getDeviceInfo(struct anotc_parsed_parameter_frame item)
{
    if (item.func==ANOTC_FRAME_DEVICE_INFO) {
        if (item.frame_value.at(0).value.uint8 & 0x1) {
            imu_status_label->setText("IMU:ON");
        }
        if (item.frame_value.at(0).value.uint8 & 0x2) {
            compass_status_label->setText("COMPASS:ON");
        }
        if (item.frame_value.at(0).value.uint8 & 0x4) {
            baro_status_label->setText("BARO:ON");
        }
    }
}

void MainWindow::flightData(struct anotc_parsed_data_frame item)
{
    if (item.func==ANOTC_FRAME_CUSTOM_SYSTEM_INFO) {
        cpu_load_label->setText(QString("CPU:%1%").arg(item.frame_value.at(1).value.uint8));
    }
}
