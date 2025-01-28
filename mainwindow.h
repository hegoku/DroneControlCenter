#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include <QMutex>
#include "Anotc/anotc.h"
#include "Anotc/anotcthread.h"
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

private slots:
    void onSerialPortConnect();
    void onSerialPortDisconnect();
    void onUDPConnect();
    void onUDPDisconnect();
    void anotcTimerHanlder();
    void showLog(_un_anotc_v8_frame);

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    AnotcThread *anotc_thread;

    QLabel *anotc_status_label;

};
#endif // MAINWINDOW_H
