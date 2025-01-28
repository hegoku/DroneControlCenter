#include "blockingqueue.h"
#include "Anotc/anotc.h"

BlockingQueue::BlockingQueue() {}

void BlockingQueue::put(union _un_anotc_v8_frame value)
{
    QMutexLocker locker(&m_mutex);
    m_queue.enqueue(value);
    m_condition.wakeOne();
}

union _un_anotc_v8_frame BlockingQueue::take()
{
    QMutexLocker locker(&m_mutex);
    while(m_queue.isEmpty()) {
        m_condition.wait(&m_mutex);
    }
    return m_queue.dequeue();
}


bool BlockingQueue::isEmpty() const
{
    QMutexLocker locker(&m_mutex);
    return m_queue.isEmpty();
}

int BlockingQueue::size() const
{
    QMutexLocker locker(&m_mutex);
    return m_queue.size();
}
