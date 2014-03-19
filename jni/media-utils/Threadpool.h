#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__


#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <bitset>
#include <iostream>


#define POOL_FREE 0
#define POOL_BUSY 1


class IIThreadNode{
public:
    IIThreadNode() {
        mState = POOL_FREE;
        mID = -1;
    }

public:
    bool isFree() {
        return mState == POOL_FREE;
    }

    int getID() {
        return mID;
    }

    void setID(int id) {
        this->mID = id;
    }

    void setState(int state){
        this->mState = state;
    }

protected:
    int mState;
    int mID;
};

template <typename T, int POOL_SIZE>
class ThreadPool
{

public:
    ThreadPool()
        : mIsClear(false)
    {

    }

public:

    int initThreadPool() {
    	std::unique_lock<std::mutex > _l(mMutex);
    	mIsClear = false;
    	mStatePool.reset();

    	for(int i=0; i<POOL_SIZE;i++) {
    		mPools[i].setState(POOL_FREE);
    	}

    	return 0;
    }

    int clearItem() {
        std::unique_lock<std::mutex > _l(mMutex);
        mIsClear = true;
        mStatePool.reset();
        mCondition.notify_all();;
        return 0;
    }

    int fetchItem(T** pItem){

        std::unique_lock<std::mutex > _l(mMutex);

        mCondition.wait(_l, [this](){
            return mIsClear||!mStatePool.all();
        });

        if (mIsClear) {
            return 1;
        }

        int i = 0;
        for(i=0; i<POOL_SIZE;i++) {
            if (!mPools[i].isFree()) {
                continue;
            }
            *pItem = (T*)&mPools[i];
            break;
        }

        if (nullptr == *pItem) {
            return -1;
        }

        mStatePool.set(i, true);
//        std::cout<<mStatePool<<std::endl;

        (*pItem)->setID(i);
        (*pItem)->setState(POOL_BUSY);

        return 0;
    }

    int refillItem(T* pItem) {
        std::unique_lock<std::mutex > _l(mMutex);
        bool isNotify = mStatePool.all();
        mStatePool.set(pItem->getID(), false);
        pItem->setState(POOL_FREE);
        if (isNotify) {
            mCondition.notify_all();
        }
        return 0;
    }

public:

    std::mutex               mMutex;
    std::condition_variable  mCondition;

    std::bitset<POOL_SIZE>   mStatePool;
    T mPools[POOL_SIZE];
    bool mIsClear;
};

#endif // THREADPOOL_H
