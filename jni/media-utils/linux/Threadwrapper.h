#ifndef __THREADWRAPPER_h__
#define __THREADWRAPPER_h__


#include "Threadevent.h"
#include "Threadguard.h"
#include <pthread.h>
#include <functional>
#include <stdint.h>


#define ThreadObj void*

typedef bool (*ThreadRunFunction) (ThreadObj);


enum ThreadPriority {
    kLowPriority = 1,
    kNormalPriority = 2,
    kHighPriority = 3,
    kHighestPriority = 4,
    kRealtimePriority = 5
};


class IThreadCallback {
public:
    virtual int onRUN(void* pObject=NULL)=0;
};

int ConvertToSystemPriority(ThreadPriority priority, int min_prio, int max_prio);

class ThreadWrapper {
protected:
    pthread_t   mThreadID;
    pid_t       mPid;
    volatile bool mIsAlive;
    volatile bool mIsDead;
    mutable MutexLock mMutexLock;
    bool mAttrScope;    // The thread competes for resources with all other threads in all processes on the system
    bool mAttrDetach;   // A thread that is created in a joinable state should eventually be joined using pthread_join。
    int  mAttrStacksize;
    bool mCancelEnable;
    bool mWaitInited;

public:
    typedef std::function<bool ()> ThreadFunctor;
    ThreadWrapper();
    virtual ~ThreadWrapper();

    bool isStarted(){return mIsAlive&&!mIsDead;}
    bool startThread();
    bool joinThread();
    bool stopThread();
    void killThread();
    void SetNotAlive();
    uint32_t GetThreadId();
    bool SetAffinity(const int* processor_numbers, const unsigned int amount_of_processors);

    virtual bool onThreadRUN(){return false;}

public:
    int threadRunFunction();
    pthread_t getId() {return mThreadID;}
    void enableScope(bool isScope) {mAttrScope=isScope; }
    void enableDetach(bool isDetach) {mAttrDetach=isDetach; }
    void setStacksize(int stacksize) {mAttrStacksize=stacksize; }

    /* SCHED_RR(compete for resource with system) */
    int setThreadPriority(ThreadPriority nPriority, int nPolicy=SCHED_RR);
    void setWaitInited(bool isWaitInited=false) {mWaitInited =isWaitInited; }
    void setThreadCancelEnble(bool cancelEnable);
    void setThreadStackszie(int nStacksize);
    void setThreadRunFunction (ThreadRunFunction threadRunFunction, ThreadObj threadObj )
         {mThreadRunFunction=threadRunFunction; mThreadObj= threadObj;}
    void setThreadFunctor(ThreadFunctor& threadFunctor){mThreadFunctor=threadFunctor;}
private:
    ThreadEvent mThreadevent;
    ThreadRunFunction mThreadRunFunction;
    ThreadObj mThreadObj;
    ThreadFunctor   mThreadFunctor;
};


#endif // __THREADWRAPPER_h__
