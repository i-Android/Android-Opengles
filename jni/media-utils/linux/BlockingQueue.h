#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include "Threadguard.h"
#include <deque>

template <typename T>

class BlockingQueue
{
public:
    BlockingQueue()
        :mMutexLock(), mCondition(mMutexLock), mQueue()
    {
    }


public:
    void put(const T& x){
        MutexLockGuard lock(mMutexLock);
        mQueue.push_back(x);
        mCondition.notify();
    }

    T take(){
        MutexLockGuard lock(mMutexLock);

        while (mQueue.empty()){
            mCondition.wait();
        }

        T front(mQueue.front());

        mQueue.pop_front();
        return front;
    }

private:
    mutable MutexLock   mMutexLock;
    ThreadCondition     mCondition;
    std::deque<T>       mQueue;
};

#endif // BLOCKINGQUEUE_H
