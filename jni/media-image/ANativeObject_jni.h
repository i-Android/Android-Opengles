/*
 * 
 */

#ifndef ANATIVEOBJECT_JNI_HH_
#define ANATIVEOBJECT_JNI_HH_

#include <jni.h>

auto    ANativeObject_getPtr(JNIEnv* env, jobject object) -> void*;

template<typename T>
auto    ANativeObject_get(JNIEnv* env, jobject object) -> T*
{
    return static_cast<T*>(ANativeObject_getPtr(env, object));
}

void    ANativeObject_set(JNIEnv* env, jobject object, void* ptr);


#endif /* ANATIVEOBJECT_JNI_HH_ */
