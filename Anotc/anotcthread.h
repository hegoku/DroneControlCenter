#ifndef ANOTCTHREAD_H
#define ANOTCTHREAD_H

#include <QThread>
#include "Anotc/anotc.h"
#include <Anotc/anotc_json.h>

class AnotcThread : public QThread
{
    Q_OBJECT

public:
    explicit AnotcThread(QObject *parent = nullptr);
    void run();

signals:
    void onFrameComing(_un_anotc_v8_frame);
    void onFlightDataComing(unsigned char func, QList<struct anotc_value>);
};

#endif // ANOTCTHREAD_H
