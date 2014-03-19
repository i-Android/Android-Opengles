#ifndef __JNI_ENGINE_H__
#define __JNI_ENGINE_H__

#include "jniDefine.h"


typedef struct _tagEGLEngine {

	jclass	glcs_eglengine;
} JNI_EGLEngine;

class JniEngine {


public:

	static jint jniSurfaceCreated(JNIEnv *env, jobject obj);

	static jint jniSurfaceChanged(JNIEnv *env, jobject obj, jint width, jint height);

	static jint jniDrawFrame(JNIEnv *env, jobject obj);

	static jint jniSetInputSource(JNIEnv *env, jobject obj, int texture, int width, int height, int rotation);

	static int initialEglEngine(JNIEnv *env);
	static int registerNatives(JNIEnv *env);
	static int unregisterNatives(JNIEnv *env);


private:
	static JNI_EGLEngine g_JniEGLEngine;
};

#endif // __JNI_ENGINE_H__
