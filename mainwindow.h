#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include <QMutex>
#include "Anotc/anotc.h"
#include "Anotc/anotcthread.h"
#include "Anotc/anotc_config_frame.h"
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void onConnect();
    void onDisconnect();

private slots:
    void onSerialPortConnect();
    void onSerialPortDisconnect();
    void onUDPConnect();
    void onUDPDisconnect();
    void anotcTimerHanlder();
    void showLog(struct anotc_blocking_queue_item);
    void onBeforeDisconnect();
    void getDeviceInfo(struct anotc_parsed_parameter_frame);
    void flightData(struct anotc_parsed_data_frame item);

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    AnotcThread *anotc_thread;

    QLabel *anotc_status_label;
    QLabel *imu_status_label;
    QLabel *compass_status_label;
    QLabel *baro_status_label;
    QLabel *cpu_load_label;

    static void sendData(const QByteArray &data);
    static void printLog(QString content);

    static MainWindow *instance;

};
#endif // MAINWINDOW_H
