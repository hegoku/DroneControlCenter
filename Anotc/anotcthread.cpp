#include "anotcthread.h"
#include "Anotc/anotc.h"
#include "Anotc/anotc_json.h"

AnotcThread::AnotcThread(QObject *parent)
    : QThread{parent}
{
}

void AnotcThread::run()
{
    union _un_anotc_v8_frame frame;
    QList<struct anotc_value> frame_value;
    for(;;) {
        frame = anotc_queue.take();
        if (anotc_frame_defination_list.contains(frame.frame.fun)) {
            frame_value.clear();
            QString value_string;
            if (anotc_parse_data_frame(&frame, &frame_value)==0) {
                emit onFlightDataComing(frame.frame.fun, frame_value);
            }
        } else {
            emit onFrameComing(frame);
        }
    }
}
