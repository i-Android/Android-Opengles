#ifndef __LW_UTILIT_H__
#define __LW_UTILIT_H__

#include <iostream>
#include <android/log.h>
#include <jni.h>

using namespace std;


class JniEnvWraper {
public:
	JniEnvWraper(JavaVM* jvm);
	~JniEnvWraper();
public:
	int AttachCurrentThread(JNIEnv **env);
	int DetachCurrentThread();
private:
	JavaVM* mJvm;
	bool mIsAttach;
public:
	static void setAndroidObject(void* javaVM){g_jvm = static_cast<JavaVM*> (javaVM);};
public:
	static JavaVM * g_jvm;
};

class JniJavaField{
public:
	static int getJavaIntField(JNIEnv *env, jobject obj, const char * name);
	static void setJavaIntField(JNIEnv *env, jobject obj, const char* name, int value);

	static unsigned char getJavaByteField(JNIEnv *env, jobject obj, const char * name);
	static void setJavaByteField(JNIEnv *env, jobject obj, const char * name, unsigned char value);

	static std::string getJavaByteArrayField(JNIEnv *env, jobject obj, const char * name);
	static void setJavaByteArrayField(JNIEnv *env, jobject obj, const char * name, const std::string &value);

	static double getJavaDoubleField(JNIEnv *env, jobject obj, const char * name);
	static void setJavaDoubleField(JNIEnv *env, jobject obj, const char * name, double value);

	static std::string getJavaStringField(JNIEnv *env, jobject obj, const char * name);
	static void setJavaStringField(JNIEnv *env, jobject obj, const char * name, const std::string &value);

	static jlong getJavaLongField(JNIEnv *env, jobject obj, const char * name);
	static void setJavaLongField(JNIEnv *env, jobject obj, const char* name, jlong value);

};


#endif //_JNI_UTILITY_H_
