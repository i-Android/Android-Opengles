#include "ThreadLooper.h"
#include "ThreadHandler.h"
//#include "ThreadPool.h"
#include "ThreadHelper.h"

#include <iostream>


using namespace laiwang;
using namespace Utils;


class MediaEvent : public IIThreadEvent {

public:
    MediaEvent() {

    }

public:
    int onEventHandler(int e, int a1){

        return 0;
    }
};

//laiwang::Utils::LooperThread<MediaMessage> mThreadLooper;

//MediaMessage mMediaMessage;




//laiwang::Utils::EventThread mEventThread;
ThreadLooper<ThreadMessage<MediaEvent>> mThreadLooper;

MediaEvent mMediaEvent;

ThreadHelper mThreadHelper;


class VideoTest {

public:
    VideoTest() {
        mTest = -1;
        mThreadHelper = ThreadHelper([this]()->int{

            return mTest;
        });
    }

public:

    int start() {
        return mThreadHelper.start();
    }
private:
    ThreadHelper mThreadHelper;

    int mTest ;
};

VideoTest mVideoTest;

int main_thread_loop() {


//    mThreadLooper.Start();
//    mThreadLooper.Post(ThreadMessage<MediaEvent>(mMediaEvent, 1,2));

//    char c = getchar();
//    mThreadLooper.Quit();
//    mThreadLooper.Stop();

//    mEventThread.Start();


//    // create thread pool with 4 worker threads
//    ThreadPool pool(4);

//    char c = getchar();

//    // enqueue and store future
//    auto result = pool.enqueue([](int answer) {
//        return answer;
//    }, 42);

//    // get result from future
//    std::cout << result.get() << std::endl;



    mThreadHelper.start();

    mVideoTest.start();

    char c = getchar();

    mThreadHelper.exist();
    return 0;
}
