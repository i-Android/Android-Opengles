#include "ThreadQueue.h"
#include <iostream>

using namespace laiwang;
using namespace Utils;


struct MsgItem {


    int i ;
};



void produce(ThreadQueue<int>& q) {
  for (int i = 0; i< 10000; ++i) {
    std::cout << "Pushing " << i << "\n";
    q.push(i);
  }
}

void consume(ThreadQueue<int>& q, unsigned int id) {
  for (int i = 0; i< 2500; ++i) {
    int item = 0;
    q.pop(item);
    std::cout << "Consumer " << id << " popped " << item << "\n";
  }
}


ThreadQueue<int> gQ;


//class AVFrameTest

int main_thread_queue() {


    using namespace std::placeholders;

    // producer thread
    std::thread prod1(std::bind(produce, std::ref(gQ)));

    // consumer threads
    std::thread consumer1(std::bind(&consume, std::ref(gQ), 1));
    std::thread consumer2(std::bind(&consume, std::ref(gQ), 2));
    std::thread consumer3(std::bind(&consume, std::ref(gQ), 3));
    std::thread consumer4(std::bind(&consume, std::ref(gQ), 4));

    prod1.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
    consumer4.join();



//    laiwang::Utils::LRUCache<int, int> cache(3);

//    cache.put(1, 2);
//    std::cout << cache.get(1) << std::endl;

//    cache.put(11, 22);

//    cache.put(111, 222);

//    cache.put(1111, 2222);

//    std::cout << cache.get(1) << std::endl;

    char c  = getchar();
    return 0;
}
