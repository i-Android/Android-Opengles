#ifndef __EVENTTHREAD_H__
#define __EVENTTHREAD_H__

#include "ThreadLooper.h"

namespace laiwang{
namespace Utils{


class IIThreadEvent {

public:
    virtual int onEventHandler(int e, int a1)=0;
};


template<typename T>
struct ThreadMessage {
    ThreadMessage(T& t, int e, int a1=0)
        : target(t),
          event(e),
          arg1(a1)
    {
    }

    void operator() () {

        target.onEventHandler(event, arg1);
    }

    T&  target;
    int event;
    int arg1;
};

//template<typename T>
//using HandlerThread = IIThreadEvent;

}
}


#endif // __EVENTTHREAD_H__
