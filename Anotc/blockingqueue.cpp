#include "blockingqueue.h"
#include "Anotc/anotc.h"

// template <class T>
// BlockingQueue<T>::BlockingQueue() {}

// template <class T>
// void BlockingQueue<T>::put(T value)
// {
//     QMutexLocker locker(&m_mutex);
//     m_queue.enqueue(value);
//     m_condition.wakeOne();
// }

// template <class T>
// T BlockingQueue<T>::take()
// {
//     QMutexLocker locker(&m_mutex);
//     while(m_queue.isEmpty()) {
//         m_condition.wait(&m_mutex);
//     }
//     return m_queue.dequeue();
// }

template <class T>
bool BlockingQueue<T>::isEmpty() const
{
    QMutexLocker locker(&m_mutex);
    return m_queue.isEmpty();
}

template <class T>
int BlockingQueue<T>::size() const
{
    QMutexLocker locker(&m_mutex);
    return m_queue.size();
}
