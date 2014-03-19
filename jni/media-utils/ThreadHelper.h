#ifndef __THREADHELPER_H__
#define __THREADHELPER_H__


#include <thread>
#include "Logginghelper.h"

namespace laiwang {
namespace Utils {


class ThreadHelper {

public:
//	ThreadHelper() {
//		mIsExist = false;
//	}
//
//	ThreadHelper(std::function<int()> pfunc) {
//		mIsExist = false;
//		mThreadFunction = pfunc;
//	}

	~ThreadHelper() {
		exit();
	}

public:

	int start(){
//		[this]()->int{this->onRun();};
		mIsExist = false;
		mThread = new std::thread(&ThreadHelper::onRun, this);
		return 0;
	}

	int exit(){
		if (!isAcivity()) {
			return 0;
		}
//
		mIsExist = true;

		mThread->join();
		mThreadFunction = nullptr;
		delete mThread;
		mThread = nullptr;
		return 0;
	}

	bool isAcivity() {
		return nullptr!=mThread && mThread->joinable();
	}

	int onRun() {

		while (!mIsExist) {

			int ret = 0;
			if (nullptr!=mThreadFunction) {
				ret = mThreadFunction();
			} else {
				ret = onHandler();
			}

			if (ret==0) {
				break;
			}
		}

		return 0;
	}

public:
	virtual int onHandler(){return 0;};

public:
	std::function<int()> mThreadFunction;

#ifdef  __ANDROID_OS__
    bool mIsExist = false;
#else
    bool mIsExist ;
#endif

	std::thread* mThread = nullptr;
};

}
}

#endif // __THREADHELPER_H__
