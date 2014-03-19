#include "jniEngine.h"
#include "gl/LWGLManager.h"

JNI_EGLEngine JniEngine::g_JniEGLEngine;


jint JniEngine::jniSurfaceCreated(JNIEnv *env, jobject obj)
{
    LOGD("jniSurfaceCreated");

    LWGLManager::getInstance()->initial();
    return 0;
}

jint JniEngine::jniSurfaceChanged(JNIEnv *env, jobject obj, jint width, jint height)
{
    LOGD("jniSurfaceChanged");

    return 0;
}

jint JniEngine::jniDrawFrame(JNIEnv *env, jobject obj)
{
    LOGD("jniDrawFrame");
    LWGLManager::getInstance()->render();
    return 0;
}

int JniEngine::initialEglEngine(JNIEnv *env) {

	LOGD("initialEglEngine");
	jclass lcls_eglengine = nullptr;

	lcls_eglengine = env->FindClass(J_EGLENGINE);
	if (!lcls_eglengine){
		return JNI_ERR;
	}
	g_JniEGLEngine.glcs_eglengine = (jclass) env->NewGlobalRef(lcls_eglengine);

	return 0;
}

jint JniEngine::jniSetInputSource(JNIEnv *env, jobject obj, int texture, int width, int height, int rotation) {

	LWGLManager::getInstance()->setInputSource(texture, width, height, rotation);
	return 0;
}

int JniEngine::registerNatives(JNIEnv *env)
{
	LOGD("registerNatives");
	JNINativeMethod jni_methods[] = {
	        { "onSurfaceCreated", "()I", (void *) JniEngine::jniSurfaceCreated },
	        { "onSurfaceChanged", "(II)I", (void *) JniEngine::jniSurfaceChanged },
	        { "onDrawFrame", "()I", (void *) JniEngine::jniDrawFrame },
	        { "setInputSource", "(IIII)I", (void *) jniSetInputSource },
	};

    if (env->RegisterNatives(g_JniEGLEngine.glcs_eglengine, jni_methods, sizeof(jni_methods) / sizeof(jni_methods[0])) < 0)
    {
        LOGE("RegisterNatives failed for '%s'", J_EGLENGINE);
        return JNI_ERR;
    }

    return JNI_OK;
}

int JniEngine::unregisterNatives(JNIEnv *env)
{
    if (g_JniEGLEngine.glcs_eglengine) {
        env->UnregisterNatives(g_JniEGLEngine.glcs_eglengine);
    }


    return 0;
}
