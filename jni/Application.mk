NDK_TOOLCHAIN_VERSION=4.8

APP_STL       	:= gnustl_static
#APP_STL    	:= gnustl_shared

#APP_CPPFLAGS 	:= 	\
#    				-D__STDC_CONSTANT_MACROS \
#    				-D__STDC_LIMIT_MACROS \
#				    -D__STDC_FORMAT_MACROS \
#				    -D__DEBUG__ \
#				    -std=c++11
				    
APP_CPPFLAGS 	+= -std=c++11

#APP_STL        := stlport_static
APP_OPTIM      	:= debug
APP_ABI         := armeabi-v7a
#APP_PLATFORM    := android-8    
APP_PLATFORM    := android-14

#MEDIA_PATH		:= /home/i-ubuntu/MyWork/MyLaiwang/laiwang_mediaplayer

MEDIA_PATH 		:= /Users/i-mac/i-jdan/MyCode/Android-Kernel/Android-Opengles




