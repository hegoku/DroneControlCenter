#ifndef ANOTCTHREAD_H
#define ANOTCTHREAD_H

#include <QThread>
#include "Anotc/anotc.h"
#include "Anotc/anotc_data_frame.h"
#include "Anotc/anotc_config_frame.h"

class AnotcThread : public QThread
{
    Q_OBJECT

public:
    explicit AnotcThread(QObject *parent = nullptr);
    void run();
    void setSendDelegate(void (*send)(const QByteArray &data));

    void (*sendDelegate)(const QByteArray &data);

    static AnotcThread *instance;
    static void anotc_send(unsigned char *data, int len);

signals:
    void onFrameComing(struct anotc_blocking_queue_item);
    void onFlightDataComing(struct anotc_parsed_data_frame);
    void onFlightParamComing(struct anotc_parsed_parameter_frame);


};

#endif // ANOTCTHREAD_H
