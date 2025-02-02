#include "anotcthread.h"
#include "Anotc/anotc.h"
#include "Anotc/anotc_data_frame.h"
#include "Anotc/anotc_config_frame.h"

AnotcThread *AnotcThread::instance = 0;

AnotcThread::AnotcThread(QObject *parent)
    : QThread{parent}
{
    AnotcThread::instance = this;
    anotc_send_func = AnotcThread::anotc_send;
}

void AnotcThread::run()
{
    struct anotc_blocking_queue_item item;
    struct anotc_parsed_data_frame parsed_data_frame;
    struct anotc_parsed_parameter_frame parsed_param_frame;
    for(;;) {
        item = anotc_queue.take();
        if (anotc_frame_defination_list.contains(item.frame.frame.fun)) {
            parsed_data_frame.timestamp = item.timestamp;
            parsed_data_frame.func = item.frame.frame.fun;
            parsed_data_frame.frame_value.clear();
            if (anotc_parse_data_frame(&item.frame, &parsed_data_frame.frame_value)==0) {
                emit onFlightDataComing(parsed_data_frame);
            }
        } else if (item.frame.frame.fun>=ANOTC_FRAME_CONFIG_CMD && item.frame.frame.fun<=ANOTC_FRAME_DEVICE_INFO) {
            parsed_param_frame.timestamp = item.timestamp;
            parsed_param_frame.func = item.frame.frame.fun;
            parsed_param_frame.frame_value.clear();
            if (anotc_parse_config_frame(&item.frame, &parsed_param_frame.frame_value)==0) {
                emit onFlightParamComing(parsed_param_frame);
            }
        } else {
            emit onFrameComing(item);
        }
    }
}

void AnotcThread::setSendDelegate(void (*send)(const QByteArray &data))
{
    sendDelegate = send;
}

void AnotcThread::anotc_send(unsigned char *data, int len)
{
    QByteArray data_to_send = QByteArray::fromRawData((char*)data, len);
    AnotcThread::instance->sendDelegate(data_to_send);
}
