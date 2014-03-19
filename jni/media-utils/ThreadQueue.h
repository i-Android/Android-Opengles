#ifndef __THREADQUEUE_H__
#define __THREADQUEUE_H__


#include <queue>
#include <list>
#include <mutex>
#include <thread>
#include <cstdint>
#include <condition_variable>

namespace laiwang {
namespace Utils {


/**
 * A thread-safe asynchronous queue
 */
template <class T>
class ThreadQueue {

public:
    ThreadQueue() = default;

    ~ThreadQueue()
    {
//        std::lock_guard<std::mutex> lock (m_mutex);
    }


public:

    int size() {
        std::lock_guard<std::mutex> lock (m_mutex);
        return m_queue.size();
    }

    int clear() {
    	 std::lock_guard<std::mutex> lock (m_mutex);
    	 // I want to avoid pop in a loop
    	 while (!m_queue.empty()){
    		 m_queue.pop();
    	 }
    	 return 0;
    }

    bool isEmpty() {
        std::lock_guard<std::mutex> lock (m_mutex);
        return m_queue.empty();
    }

    int push (const T& item)
    {
        std::lock_guard<std::mutex> lock (m_mutex);
        m_queue.push (item);
        return 0;
    }

    int front(T& item) {
        std::unique_lock<std::mutex> lock (m_mutex);
        if (m_queue.empty()) {
        	item = nullptr;
            return -1;
        }
        item = m_queue.front();
        return 0;
    }

    int pop (T& item)
    {
        std::unique_lock<std::mutex> lock (m_mutex);
        if (m_queue.empty()) {
            return -1;
        }
        item = m_queue.front();
        m_queue.pop();
        return 0;
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
};

}
}


#endif // THREADQUEUE_H
