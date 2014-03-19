#include "jniUtil.h"
#include "jniOnload.h"
#include "jniEngine.h"


JavaVM * gvm;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved){
	JNIEnv *env;
	gvm = jvm;

	if(jvm->GetEnv((void**)&env, JNI_VERSION_1_2) != JNI_OK){
		return -1;
	}

	LOGD("JNI_OnLoad");
	JniEnvWraper::setAndroidObject(jvm);

//	ANativeObject_jni(env);
	JniEngine::initialEglEngine(env);
	JniEngine::registerNatives(env);

	return JNI_VERSION_1_2;
}
