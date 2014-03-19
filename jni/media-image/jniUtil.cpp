#include "jniUtil.h"


JavaVM* JniEnvWraper::g_jvm = NULL;


JniEnvWraper::JniEnvWraper(JavaVM* jvm):mJvm(jvm){

}

JniEnvWraper::~JniEnvWraper(){
	DetachCurrentThread();
}

int JniEnvWraper::AttachCurrentThread(JNIEnv **env){

	// get the JNI env for this thread
	mIsAttach = false;
	if (mJvm->GetEnv((void**) env, JNI_VERSION_1_2) != JNI_OK) {
		// try to attach the thread and get the env
		// Attach this thread to JVM
		jint res = mJvm->AttachCurrentThread(env, NULL);
		if ((res < 0) || !env) {
			return -1;
		}

		mIsAttach = true;
	}
	return 1;
}


int JniEnvWraper::DetachCurrentThread(){
	int nRet = 0;
	if (mIsAttach) {
		nRet = mJvm->DetachCurrentThread();
	}

	return nRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

int JniJavaField::getJavaIntField(JNIEnv *env, jobject obj, const char * name){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "I");
	env->DeleteLocalRef(cls);
	return env->GetIntField(obj, fid);
}

void JniJavaField::setJavaIntField(JNIEnv *env, jobject obj, const char* name, int value){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "I");
	env->SetIntField(obj, fid, (jint)value);
	env->DeleteLocalRef(cls);
}

unsigned char JniJavaField::getJavaByteField(JNIEnv *env, jobject obj, const char * name){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "B");
	env->DeleteLocalRef(cls);
	return env->GetByteField(obj, fid);
}

void JniJavaField::setJavaByteField(JNIEnv *env, jobject obj, const char * name, unsigned char value){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "B");
	env->DeleteLocalRef(cls);
	env->SetByteField(obj, fid, (jbyte)value);
}

std::string JniJavaField::getJavaByteArrayField(JNIEnv *env, jobject obj, const char * name){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "[B");
	env->DeleteLocalRef(cls);

	jbyteArray fobj = (jbyteArray)env->GetObjectField(obj, fid);
	if(fobj == NULL){
		return "";
	}
	jbyte *bytearray = env->GetByteArrayElements(fobj, NULL);
	int len = env->GetArrayLength(fobj);

	string stdstr;
	stdstr.reserve(len);
	stdstr.append((const char*)bytearray, len);

	env->ReleaseByteArrayElements(fobj, bytearray, JNI_ABORT);
	env->DeleteLocalRef(fobj);
	return stdstr;
}

void JniJavaField::setJavaByteArrayField(JNIEnv *env, jobject obj, const char * name, const std::string &value){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "[B");

	jbyteArray fobj = env->NewByteArray(value.length());

	env->SetByteArrayRegion(fobj, 0, value.length(), reinterpret_cast<const signed char*>(value.c_str()));
	env->SetObjectField(obj, fid, fobj);

	env->DeleteLocalRef(cls);
	env->DeleteLocalRef(fobj);
}

double JniJavaField::getJavaDoubleField(JNIEnv *env, jobject obj, const char * name){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "D");
	env->DeleteLocalRef(cls);
	return env->GetDoubleField(obj, fid);
}


void JniJavaField::setJavaDoubleField(JNIEnv *env, jobject obj, const char * name, double value){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "D");
	env->DeleteLocalRef(cls);
	env->SetDoubleField(obj, fid, (jdouble)value);
}

string JniJavaField::getJavaStringField(JNIEnv *env, jobject obj, const char * name){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "Ljava/lang/String;");
	env->DeleteLocalRef(cls);

	jstring jstr = (jstring)env->GetObjectField(obj, fid);
	if(jstr == NULL){
		return "";
	}
	const char* str = env->GetStringUTFChars(jstr, NULL);
	string stdstr = str;
	env->ReleaseStringUTFChars(jstr, str);
	env->DeleteLocalRef(jstr);
	return stdstr;
}

void JniJavaField::setJavaStringField(JNIEnv *env, jobject obj, const char * name, const std::string &value){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "Ljava/lang/String;");
	env->DeleteLocalRef(cls);

	jstring jvalue = env->NewStringUTF(value.c_str());
	env->SetObjectField(obj, fid, jvalue);
	env->DeleteLocalRef(jvalue);
}

jlong JniJavaField::getJavaLongField(JNIEnv *env, jobject obj, const char * name){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "J");
	env->DeleteLocalRef(cls);
	return env->GetLongField(obj, fid);
}

void JniJavaField::setJavaLongField(JNIEnv *env, jobject obj, const char* name, jlong value){
	jclass cls = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(cls, name, "J");
	env->SetLongField(obj, fid, value);
	env->DeleteLocalRef(cls);
}


