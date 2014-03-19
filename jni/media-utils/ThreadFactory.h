#ifndef __THREADSAFEMAP_H__
#define __THREADSAFEMAP_H__

#include <iostream>
#include <memory>
#include <string>
#include <map>

#include "Threadguard.h"

template<typename T>
class ThreadFactory {
public:
	std::shared_ptr<T> get(const std::string& key, bool default_null_new = false) {
		std::shared_ptr<T> spItem;
		MutexLockGuard lock(mMutexLock);
		std::weak_ptr<T>& wkItem = mSafeMap[key];
		spItem = wkItem.lock();
		if (!spItem) {
			if (!default_null_new) {
				return spItem;
			} else {
				spItem.reset(new T(key));
				wkItem = spItem; // update weak ptr
			}
		}
		return spItem;
	}

	bool Add(const std::string& key, std::shared_ptr<T>& value) {

		std::shared_ptr<T> spItem;
		MutexLockGuard lock(mMutexLock);
		std::weak_ptr<T>& wkItem = mSafeMap[key];
		spItem = wkItem.lock();
		if (spItem) {
			return false;
		} else {
			wkItem = value; // update weak ptr
		}
		return true;
	}

	int del(const std::string& key) {
		MutexLockGuard lock(mMutexLock);
		return mSafeMap.erase(key);
	}
private:
	std::map<std::string, std::weak_ptr<T> > mSafeMap;
	mutable MutexLock mMutexLock;
};

#endif // __THREADSAFEMAP_H__
