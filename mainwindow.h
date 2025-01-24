#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include <QMutex>
#include "Anotc/anotc.h"

Q_DECLARE_METATYPE(union _un_anotc_v8_frame);

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
    void showLog(_un_anotc_v8_frame*);

signals:
    void onDataComing(_un_anotc_v8_frame*);

private:
    Ui::MainWindow *ui;

    // static void anotc_handler(union _un_anotc_v8_frame *frame);
    // static QList<union _un_anotc_v8_frame> anotc_queue;
    static QMutex anotc_queue_mutex;

    QTimer *timer;

};
#endif // MAINWINDOW_H
