#ifndef __THREADCURRENT_H__
#define __THREADCURRENT_H__

namespace CurrentThread
{
  // internal
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread const char* t_threadName;
    void cacheTid();

    inline int tid()
    {
        if (t_cachedTid == 0)
        {
            cacheTid();
        }
        return t_cachedTid;
    }

    inline const char* tidString() // for logging
    {
        return t_tidString;
    }

    inline const char* name()
    {
        return t_threadName;
    }

    bool isMainThread();
}

#endif // __THREADCURRENT_H__
