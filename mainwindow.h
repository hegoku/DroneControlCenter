#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include "Anotc/anotc.h"

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

private:
    Ui::MainWindow *ui;

    static void anotc_handler(union _un_anotc_v8_frame *frame);
    static QList<union _un_anotc_v8_frame> anotc_queue;

    QTimer *timer;

};
#endif // MAINWINDOW_H
