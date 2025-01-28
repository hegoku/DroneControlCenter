#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <QWaitCondition>
#include <QQueue>
#include <QMutex>
#include <QThread>

class BlockingQueue
{
public:
    BlockingQueue();

    void put(union _un_anotc_v8_frame value);
    union _un_anotc_v8_frame take();

    bool isEmpty() const;
    int size() const;

private:
    QQueue<union _un_anotc_v8_frame> m_queue;
    mutable QMutex m_mutex;
    QWaitCondition m_condition;
};

#endif // BLOCKINGQUEUE_H
