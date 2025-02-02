#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <QWaitCondition>
#include <QQueue>
#include <QMutex>
#include <QThread>

template <class T>
class BlockingQueue
{
public:
    BlockingQueue() {

    }

    void put(T value) {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue(value);
        m_condition.wakeOne();
    }

    T take() {
        QMutexLocker locker(&m_mutex);
        while(m_queue.isEmpty()) {
            m_condition.wait(&m_mutex);
        }
        return m_queue.dequeue();
    }

    bool isEmpty() const;
    int size() const;

private:
    QQueue<T> m_queue;
    mutable QMutex m_mutex;
    QWaitCondition m_condition;
};

#endif // BLOCKINGQUEUE_H
