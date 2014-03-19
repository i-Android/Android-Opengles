#include "Threadwrapper.h"
#include "ThreadCurrent.h"
#include <assert.h>
#include <pthread.h>
#include <syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <iostream>

using namespace std;

namespace CurrentThread
{
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread const char* t_threadName = "unknown";
}

pid_t gettid()
{
//    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = gettid();
        int n = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
        assert(n == 6); (void) n;
    }
}

bool CurrentThread::isMainThread()
{
//    return tid() == ::getpid();
}

int ConvertToSystemPriority(ThreadPriority priority, int min_prio, int max_prio) {
    assert(max_prio - min_prio > 2);
    const int top_prio = max_prio - 1;
    const int low_prio = min_prio + 1;

    switch (priority) {
        case kLowPriority:
            return low_prio;
        case kNormalPriority:
            // The -1 ensures that the kHighPriority is always greater or equal to
            // kNormalPriority.
            return (low_prio + top_prio - 1) / 2;
        case kHighPriority:
            return std::max(top_prio - 2, low_prio);
        case kHighestPriority:
            return std::max(top_prio - 1, low_prio);
        case kRealtimePriority:
            return top_prio;
    }
    assert(false);
    return low_prio;
}

static void* threadFunction(void* args) {
    ThreadWrapper* t = static_cast<ThreadWrapper*>(args);
    t->threadRunFunction();

    return NULL;
}

ThreadWrapper::ThreadWrapper() :
    mPid(-1),
    mThreadID(0),
    mIsAlive(false),
    mIsDead(true),
    mAttrScope(false),
    mAttrDetach(false),
    mAttrStacksize(0),
    mCancelEnable(false),
    mThreadRunFunction(NULL),
    mMutexLock(),
    mWaitInited(true)
{

}

ThreadWrapper::~ThreadWrapper() {
}

uint32_t ThreadWrapper::GetThreadId() {
#if defined(WX_ANDROID) || defined(WX_LINUX)
    return static_cast<uint32_t>(syscall(__NR_gettid));
#else
    return static_cast<uint32_t>(pthread_self());
#endif
}


bool ThreadWrapper::startThread() {
    assert(mThreadID == 0);

    if (mWaitInited){
        mThreadevent.Create();
    }

    pthread_attr_t attr;

    /*初始化属性值，均设为默认值*/
    pthread_attr_init(&attr);
    if (mAttrScope) {
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    }

    if (mAttrDetach) {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }else {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    }

    sched_param param;
    pthread_attr_getschedparam(&attr, &param);
    param.sched_priority=20;
    pthread_attr_setschedparam(&attr, &param);

    if (mAttrStacksize>0) {
        pthread_attr_setstacksize(&attr, mAttrStacksize);
    }

    if (mWaitInited) {
        mThreadevent.Reset();
    }

    if (pthread_create(&mThreadID, &attr, threadFunction, this) != 0) {
        assert(0 && "Can't not create thread!");
        return false;
    }

    {
        MutexLockGuard threadCondition(mMutexLock);
        mIsDead = false;
    }

    if (mWaitInited) {
        mThreadevent.Wait(EVENT_10_SEC);
    }

    return true;
}

bool ThreadWrapper::joinThread() {
    if (mThreadID == 0) {
        return false;
    }

    void* stat;
    if (mThreadID != pthread_self()) {
        int ret = pthread_join(mThreadID, &stat);
        if (ret != 0) {
            assert(0 && "Fail to join thread");
            return false;
        }
    }
    mThreadID = 0;

    return true;
}


bool ThreadWrapper::SetAffinity(const int* processor_numbers, const unsigned int amount_of_processors) {
    if (!processor_numbers || (amount_of_processors == 0)) {
        return false;
    }
    cpu_set_t mask;
    CPU_ZERO(&mask);

    for (unsigned int processor = 0; processor < amount_of_processors; ++processor) {
        CPU_SET(processor_numbers[processor], &mask);
    }
#if defined(WX_ANDROID)
    // Android.
    const int result = syscall(__NR_sched_setaffinity, mPid, sizeof(mask), &mask);
#else
    // "Normal" Linux.
    const int result = sched_setaffinity(mPid, sizeof(mask), &mask);
#endif
    if (result != 0) {
        return false;
    }
    return true;
}

void ThreadWrapper::killThread(){
    pthread_cancel(mThreadID);
}

void ThreadWrapper::SetNotAlive() {
    MutexLockGuard threadCondition(mMutexLock);
    mIsAlive = false;
}

bool ThreadWrapper::stopThread() {

    bool dead = false;
    {
        MutexLockGuard threadCondition(mMutexLock);
        mIsAlive = false;
        dead = mIsDead;
    }

    // TODO(hellner) why not use an event here?
    // Wait up to 10 seconds for the thread to terminate
    for (int i = 0; i < 1000 && !dead; ++i) {
//        sleep(10);
        {
            MutexLockGuard threadCondition(mMutexLock);
            dead = mIsDead;
        }
    }
    if (dead) {
        return true;
    } else {
        return false;
    }
}

void ThreadWrapper::setThreadStackszie(int nStacksize) {
    // Set the stack stack size to 1M.
//    pthread_attr_setstacksize(&mThreadAttr, nStacksize);
    mAttrStacksize = nStacksize;
}

void ThreadWrapper::setThreadCancelEnble(bool cancelEnable){
    mCancelEnable = cancelEnable;
}

int ThreadWrapper::setThreadPriority(ThreadPriority nPriority, int nPolicy/*=SCHED_RR*/) {
    sched_param param;
    const int min_prio = sched_get_priority_min(nPolicy);
    const int max_prio = sched_get_priority_max(nPolicy);

    if ((min_prio == EINVAL) || (max_prio == EINVAL)) {
//        WEBRTC_TRACE(kTraceError, kTraceUtility, -1, "unable to retreive min or max priority for threads");
       return -1;
    }
    if (max_prio - min_prio <= 2) {
        // There is no room for setting priorities with any granularity.
       return -1;
    }
    param.sched_priority = ConvertToSystemPriority(nPriority, min_prio, max_prio);
    int result = pthread_setschedparam(mThreadID, nPolicy, &param);

    return result;
}

int ThreadWrapper::threadRunFunction(){

    {
       MutexLockGuard threadCondition(mMutexLock);
       mIsAlive = true;
    }
    if (mWaitInited) {
        mThreadevent.Set();
    }

    if (mCancelEnable) {
        // Enable immediate cancellation
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    }

    bool alive = true;
    bool run = true;
    while (alive) {
        run = this->onThreadRUN();

        if (mThreadRunFunction!=NULL) {
           run =  mThreadRunFunction(mThreadObj);
        }

        if (mThreadFunctor) {
           run = mThreadFunctor();
        }


        MutexLockGuard threadCondition(mMutexLock);
        if (!run) {
          mIsAlive = false;
        }
        alive = mIsAlive;
    }

    {
        MutexLockGuard threadCondition(mMutexLock);
        mIsDead = true;
    }

    cout<<"thread exit"<<endl;

    return 0;
}
