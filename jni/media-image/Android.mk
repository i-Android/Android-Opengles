LOCAL_PATH      	:= $(call my-dir)

include $(MEDIA_PATH)/jni/media-utils/Build-Android.mk

include $(CLEAR_VARS)

INET_PATH       	:= $(LOCAL_PATH)

LOCAL_MODULE       	:= lwglEngine
LOCAL_CPPFLAGS     	+= -frtti -fexceptions -fpermissive -Wno-pmf-conversions -std=c++0x -D__ANDROID_OS__ 
 
		
LOCAL_SRC_FILES 	+= $(call import_subdir_cpp_file_under)
				         		
LOCAL_LDLIBS  		+= -pthread  -lm -lz -llog  -landroid -lGLESv2 -lEGL -lGLESv1_CM 

include $(BUILD_SHARED_LIBRARY)