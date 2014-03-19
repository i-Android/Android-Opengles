#ifndef __THREAD_GUARD_H__
#define __THREAD_GUARD_H__

#include <pthread.h>
#include <errno.h>

class MutexLock{

private:
    MutexLock (const MutexLock&);
    MutexLock& operator= (const MutexLock&);

public:
    MutexLock() {
        pthread_mutex_init(&mMutex, NULL);
    }

    virtual ~MutexLock() {
        pthread_mutex_destroy(&mMutex);
    }

    pthread_mutex_t* getPthreadMutex(){
        return &mMutex;
    }

    inline void writelock() {
        pthread_mutex_lock(&mMutex);
    }

    inline void unlock() {
        pthread_mutex_unlock(&mMutex);
    }

    inline bool trylock() {

        return pthread_mutex_trylock(&mMutex) == 0;
    }

    pthread_mutex_t& getMutex() {
        return mMutex;
    }
private:
    pthread_mutex_t mMutex;
};

class MutexLockGuard{
public:
    explicit MutexLockGuard(MutexLock& mutexLock)
        :mMutexLock(mutexLock){
        mMutexLock.writelock();
    }

    ~MutexLockGuard(){
        mMutexLock.unlock();
    }

private:
    MutexLock& mMutexLock;
};

class ThreadCondition{

public:
    explicit ThreadCondition (MutexLock & mutexLock)
        :mMutex(mutexLock)
    {
        pthread_cond_init(&mCond, NULL);
    }

    ~ThreadCondition(){
        pthread_cond_destroy(&mCond);
    }

    void wait(){
        pthread_cond_wait(&mCond, mMutex.getPthreadMutex());
    }

    bool waitForSeconds(int seconds) {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += seconds;
        return ETIMEDOUT == pthread_cond_timedwait(&mCond, mMutex.getPthreadMutex(), &abstime);
    }

    void notify(){
        pthread_cond_signal(&mCond);
    }

    void notifyAll(){
        pthread_cond_broadcast(&mCond);
    }

private:
    MutexLock&      mMutex;
    pthread_cond_t  mCond;
};



#endif // __THREAD_GUARD_H__
