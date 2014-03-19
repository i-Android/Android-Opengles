#include "ANativeObject_jni.h"

#include "jniOnload.h"

static struct {
    jfieldID _Ptr;
} _ANativeObject;

auto    ANativeObject_getPtr(JNIEnv* env, jobject object) -> void*
{
    return (void*) env->GetIntField(object, _ANativeObject._Ptr);
}


void    ANativeObject_set(JNIEnv* env, jobject object, void* ptr)
{
    env->SetIntField(object, _ANativeObject._Ptr, jint(ptr));
}

void    ANativeObject_jni(JNIEnv* env)
{
    jclass klass = env->FindClass(J_ANativeObject);
    _ANativeObject._Ptr = env->GetFieldID(klass, "_Ptr", J_INT);
}
