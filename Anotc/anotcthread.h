#ifndef ANOTCTHREAD_H
#define ANOTCTHREAD_H

#include <QThread>
#include <QList>
#include <QTimer>
#include <QMutex>
#include "Anotc/anotc.h"
#include "Anotc/anotc_data_frame.h"
#include "Anotc/anotc_config_frame.h"
#include "Anotc/anotc_receive_check.h"

struct anotc_timeout {
    unsigned int try_count;
    struct anotc_frame *frame;
    int (*check_func)(struct anotc_frame *send_frame, struct anotc_frame *recv_frame);
};

class AnotcThread : public QThread
{
    Q_OBJECT

public:
    explicit AnotcThread(QObject *parent = nullptr);
    ~AnotcThread();

    void run();
    void setSendDelegate(void (*send)(const QByteArray &data));

    void (*sendDelegate)(const QByteArray &data);

    static AnotcThread *instance;
    static void anotc_send(unsigned char *data, int len);

signals:
    void onFrameComing(struct anotc_blocking_queue_item);
    void onFlightDataComing(struct anotc_parsed_data_frame);
    void onFlightParamComing(struct anotc_parsed_parameter_frame);
    void onTimerStop();
    void onCheckFrameComing(struct anotc_parsed_check_frame);
    void onCMDResponseComing(struct anotc_blocking_queue_item);

private slots:
    void checkTimeout();
    void stopTimer();

private:
    static QList<struct anotc_timeout*> timeout_queue;
    static QMutex mutex;
    static QTimer *timer;
    static void deleteTimeout(struct anotc_frame *frame);

};

#endif // ANOTCTHREAD_H
