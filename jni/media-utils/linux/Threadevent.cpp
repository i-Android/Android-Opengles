
#include "Threadevent.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>


const long int E6 = 1000000;
const long int E9 = 1000 * E6;


ThreadEvent::ThreadEvent()
{


}

ThreadEvent::~ThreadEvent(){
    //pthread_cond_destroy(&mCond);
    //pthread_mutex_destroy(&mMutex);

}

int ThreadEvent::Create() {

    int result = pthread_mutex_init(&mMutex, 0);


    if (result != 0) {
        return -1;
    }

#ifdef THREAD_CLOCK_TYPE_REALTIME
    result = pthread_cond_init(&mCond, null);
    if (result != 0) {
        return -1;
    }
#else

    pthread_condattr_t cond_attr;

    result = pthread_condattr_init(&cond_attr);

    if (result != 0) {
        return -1;
    }

    result = pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC);

    if (result != 0) {
        return -1;
    }

    result = pthread_cond_init(&mCond, &cond_attr);
    if (result != 0) {
        return -1;
    }
    result = pthread_condattr_destroy(&cond_attr);
    if (result != 0) {
        return -1;
    }
#endif

    return 0;
}


bool ThreadEvent::Reset(){
    if (0 != pthread_mutex_lock(&mMutex)) {
        return false;
    }

    mEventState = kDown;
    pthread_mutex_unlock(&mMutex);

    return true;
}

bool ThreadEvent::Set() {
    if (0 != pthread_mutex_lock(&mMutex)) {
        return false;
    }
    mEventState = kUp;
    pthread_cond_broadcast(&mCond);
    pthread_mutex_unlock(&mMutex);
    return true;
}

EventType ThreadEvent::Wait(unsigned long timeout){

    int ret_val = 0;

    if (0 != pthread_mutex_lock(&mMutex) ) {
        return kEventError;
    }

    if (kDown == mEventState) {
        if (EVENT_INFINITE != timeout) {
            timespec end_at;
#ifdef MAC
#ifdef THREAD_CLOCK_TYPE_REALTIME
            clock_gettime(CLOCK_REALTIME, &end_at);
#else
            clock_gettime(CLOCK_MONOTONIC, &end_at);
#endif
#else
            struct timeval value;
            struct timezone time_zone;
            time_zone.tz_minuteswest = 0;
            time_zone.tz_dsttime = 0;
            gettimeofday(&value, &time_zone);
            TIMEVAL_TO_TIMESPEC(&value, &end_at);

#endif
            end_at.tv_sec += timeout / 1000;
            end_at.tv_nsec += (timeout - (timeout/1000) * 1000 ) * E6;
            if (end_at.tv_nsec >= E9){
                end_at.tv_sec ++ ;
                end_at.tv_nsec -= E9;
            }
            ret_val = pthread_cond_timedwait(&mCond, &mMutex, &end_at);

        } else {
            ret_val = pthread_cond_wait(&mCond, &mMutex);
        }

    }

    mEventState = kDown;
    pthread_mutex_unlock(&mMutex);


    switch (ret_val) {
    case 0:
        return kEventSignaled;
    case ETIMEDOUT:
        return kEventTimeout;
    default:
        return kEventError;
    }

}
