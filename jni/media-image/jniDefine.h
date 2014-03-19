#ifndef __JNI_DEFINE_H__
#define __JNI_DEFINE_H__

#include <jni.h>
#include "../media-utils/Logginghelper.h"
#define LOG_TAG "lwglEngine"

#define J_BYTE          "B"
#define J_INT           "I"
#define J_VOID          "V"
#define J_BOOL          "Z"
#define J_LONG          "J"
#define J_String        "java/lang/String"
#define J_ByteBuffer    "java/nio/ByteBuffer"
#define J_IOException   "java/io/IOException"

#define J_Bitmap                "android/graphics/Bitmap"

#define J_PACKAGE 				"com/alibaba/android/babylon/"
#define J_EGLENGINE 			J_PACKAGE"graphic/LWGLEngine"


#define J_ANativeObject         "com/duanqu/jni/ANativeObject"


#define J_(klass)   "L" klass ";"
#define JA_(type)   "[" type

#define J_METHOD(klass, method, ret, ...) { #method, "(" __VA_ARGS__ ")" ret, (void*) klass ##_## method}

#endif //_JNI_UTILITY_H_
