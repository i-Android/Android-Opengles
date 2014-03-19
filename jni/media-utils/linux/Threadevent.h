#ifndef __THREAD_EVENT_H__
#define __THREAD_EVENT_H__

#include <pthread.h>



enum EventType {
    kEventSignaled = 1,
    kEventError = 2,
    kEventTimeout = 3
};

enum EventState {
    kUp = 1,
    kDown = 2
};


#define EVENT_10_SEC    10000
#define EVENT_INFINITE  0xffffff

class ThreadEvent
{
public:
    ThreadEvent();
    virtual ~ThreadEvent();

public:
    int  Create();
    bool Reset();
    bool Set();
    EventType Wait(unsigned long timeout);

private:
    pthread_mutex_t mMutex;
    pthread_cond_t  mCond;

    bool mClockTypeRealtime;

    EventState mEventState;

};

#endif // __THREAD_EVENT_H__
