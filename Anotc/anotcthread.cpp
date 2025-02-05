#include "anotcthread.h"
#include "Anotc/anotc.h"
#include "Anotc/anotc_data_frame.h"
#include "Anotc/anotc_config_frame.h"
#include "Anotc/anotc_receive_check.h"
#include "DLog.h"

AnotcThread *AnotcThread::instance = 0;
QTimer *AnotcThread::timer = new QTimer();


AnotcThread::AnotcThread(QObject *parent)
    : QThread{parent}
{
    AnotcThread::instance = this;
    anotc_send_func = AnotcThread::anotc_send;
    // timer = new QTimer();
    timer->setInterval(2000);
    timer->setSingleShot(true);
    connect(this, &AnotcThread::onTimerStop, this, &AnotcThread::stopTimer);
    connect(timer, &QTimer::timeout, this, &AnotcThread::checkTimeout);
    // timer->start();
}

AnotcThread::~AnotcThread()
{
    timer->stop();
}

void AnotcThread::stopTimer()
{
    timer->stop();
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
                deleteTimeout(&item.frame.frame);
                emit onFlightParamComing(parsed_param_frame);
            }
        } else if (item.frame.frame.fun==ANOTC_FRAME_FRAME_CHECK) {
            if (item.frame.frame.data[0]>=ANOTC_FRAME_CONFIG_CMD && item.frame.frame.data[0]<=ANOTC_FRAME_DEVICE_INFO) {
                deleteTimeout(&item.frame.frame);
            }
            emit onFrameComing(item);
        } else {
            emit onFrameComing(item);
        }
    }
}

void AnotcThread::setSendDelegate(void (*send)(const QByteArray &data))
{
    sendDelegate = send;
}

QList<struct anotc_timeout*> AnotcThread::timeout_queue;

void AnotcThread::anotc_send(unsigned char *data, int len)
{
    struct anotc_frame *frame = (struct anotc_frame*)data;
    struct anotc_frame *send_frame;
    struct anotc_timeout *timeout;
    AnotcThread::mutex.lock();
    if (frame->fun==ANOTC_FRAME_CONFIG_CMD) {
        if (frame->data[0]==ANOTC_CONFIG_FRAME_CMD_DEVICE_INFO) {
            send_frame = (struct anotc_frame*)malloc(sizeof(struct anotc_frame));
            memcpy(send_frame, frame, sizeof(struct anotc_frame));
            timeout = (struct anotc_timeout*)malloc(sizeof(struct anotc_timeout));
            timeout->try_count = 5;
            timeout->frame = send_frame;
            timeout->check_func = check_recv_device_info;
            timeout_queue.append(timeout);
        } else if (frame->data[0]==ANOTC_CONFIG_FRAME_CMD_READ_COUNT) {
            send_frame = (struct anotc_frame*)malloc(sizeof(struct anotc_frame));
            memcpy(send_frame, frame, sizeof(struct anotc_frame));
            timeout = (struct anotc_timeout*)malloc(sizeof(struct anotc_timeout));
            timeout->try_count = 5;
            timeout->frame = send_frame;
            timeout->check_func = check_recv_param_count;
            timeout_queue.append(timeout);
        } else if (frame->data[0]==ANOTC_CONFIG_FRAME_CMD_READ_VALUE) {
            send_frame = (struct anotc_frame*)malloc(sizeof(struct anotc_frame));
            memcpy(send_frame, frame, sizeof(struct anotc_frame));
            timeout = (struct anotc_timeout*)malloc(sizeof(struct anotc_timeout));
            timeout->try_count = 5;
            timeout->frame = send_frame;
            timeout->check_func = check_recv_param_value;
            timeout_queue.append(timeout);
        } else if (frame->data[0]==ANOTC_CONFIG_FRAME_CMD_READ_INFO) {
            send_frame = (struct anotc_frame*)malloc(sizeof(struct anotc_frame));
            memcpy(send_frame, frame, sizeof(struct anotc_frame));
            timeout = (struct anotc_timeout*)malloc(sizeof(struct anotc_timeout));
            timeout->try_count = 5;
            timeout->frame = send_frame;
            timeout->check_func = check_recv_param_info;
            timeout_queue.append(timeout);
        }
        if (!timer->isActive()) {
            timer->start();
        }
    } else if (frame->fun==ANOTC_FRAME_CONFIG_READ_WRITE) {
        send_frame = (struct anotc_frame*)malloc(sizeof(struct anotc_frame));
        memcpy(send_frame, frame, sizeof(struct anotc_frame));
        timeout = (struct anotc_timeout*)malloc(sizeof(struct anotc_timeout));
        timeout->try_count = 5;
        timeout->frame = send_frame;
        timeout->check_func = check_recv_param_set_value;
        timeout_queue.append(timeout);
        if (!timer->isActive()) {
            timer->start();
        }
    }
    AnotcThread::mutex.unlock();
    QByteArray data_to_send = QByteArray::fromRawData((char*)data, len);
    AnotcThread::instance->sendDelegate(data_to_send);
}

QMutex AnotcThread::mutex;

void AnotcThread::checkTimeout()
{
    AnotcThread::mutex.lock();
    QList<int> deleteIndex;
    bool has_count_zero = false;
    do {
        has_count_zero = false;
        for (int i=0;i<AnotcThread::timeout_queue.count();i++) {
            if (AnotcThread::timeout_queue.at(i)->try_count==0) {
                has_count_zero = true;
                DLogW(QString("timeout:%1").arg(AnotcThread::timeout_queue.at(i)->frame->fun));
                free(AnotcThread::timeout_queue.at(i)->frame);
                free(AnotcThread::timeout_queue.at(i));
                AnotcThread::timeout_queue.removeAt(i);
                break;
            }
        }
    } while(has_count_zero);
    for (int i=0;i<AnotcThread::timeout_queue.count();i++) {
        QByteArray data_to_send = QByteArray::fromRawData((char*)AnotcThread::timeout_queue.at(i)->frame, AnotcThread::timeout_queue.at(i)->frame->len+ANOTC_V8_HEAD_SIZE+2);
        AnotcThread::instance->sendDelegate(data_to_send);
        DLogW(QString("f%1 try count:%2").arg(AnotcThread::timeout_queue.at(i)->frame->fun).arg(AnotcThread::timeout_queue.at(i)->try_count));
        AnotcThread::timeout_queue.at(i)->try_count--;
    }

    if (AnotcThread::timeout_queue.count()>0) {
        timer->start();
    }
    AnotcThread::mutex.unlock();
}

void AnotcThread::deleteTimeout(struct anotc_frame *frame)
{
    AnotcThread::mutex.lock();
    for (int i=0;i<AnotcThread::timeout_queue.count();i++) {
        if (AnotcThread::timeout_queue.at(i)->check_func(AnotcThread::timeout_queue.at(i)->frame, frame)==0) {
            free(AnotcThread::timeout_queue.at(i)->frame);
            free(AnotcThread::timeout_queue.at(i));
            AnotcThread::timeout_queue.removeAt(i);
            break;
        }
    }
    if (AnotcThread::timeout_queue.count()==0) {
        emit AnotcThread::instance->onTimerStop();
        // timer->stop();
    }
    AnotcThread::mutex.unlock();
}
