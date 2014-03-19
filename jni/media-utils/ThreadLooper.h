#ifndef __LOOPER_H__
#define __LOOPER_H__

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace laiwang {
namespace Utils{

using std::mutex;
using std::condition_variable;
using std::queue;
using std::lock_guard;
using std::unique_lock;
using std::thread;

template<typename MSG>
class Looper {

private:
    mutex               _Mutex;
    condition_variable  _Condition;
    bool                _Quit ;
    queue<MSG>          _Queue;

public:
    void Post(MSG msg)
    {
        lock_guard<mutex> _l(_Mutex);
        bool notify = _Queue.empty();
        _Queue.push(msg);
        if (notify) {
            _Condition.notify_one();
        }
    }

    void Quit()
    {
        lock_guard<mutex> _l(_Mutex);
        _Quit = true;
        if (_Queue.empty()) {
            _Condition.notify_one();
        }
    }

    auto Reset() -> bool
    {
        lock_guard<mutex> _l(_Mutex);

        if (!_Quit || !_Queue.empty()) {
            return false;
        }
        _Quit = false;

        return true;
    }

public:
    void Loop()
    {
        do {
            queue<MSG> batch;

            {
                unique_lock<mutex> _l(_Mutex);
                if (_Queue.empty()) {
                    if (_Quit) {
                        break;
                    }
                    _Condition.wait(_l);
                    continue;
                }
                batch.swap(_Queue);
            }

            while (!batch.empty()) {
                batch.front()();
                batch.pop();
            }
        } while (true);
    }
};

template<typename MSG>
class ThreadLooper : Looper<MSG> {
public:
    void Start()
    {
        _Thread = thread(&ThreadLooper::ThreadLooperFunc, this);
    }

    void Stop()
    {
        _Thread.join();
        _Thread = thread();

        Looper<MSG>::Reset();
    }

    auto Active() const -> bool { return _Thread.joinable(); }

    using Looper<MSG>::Post;
    using Looper<MSG>::Quit;

private:
    void ThreadLooperFunc()
    {
        Looper<MSG>::Loop();
    }

private:;
    thread      _Thread;
};

} // namespace utils
} // namespace laiwang

#endif // __LOOPER_H__
