LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellomk
LOCAL_SRC_FILES += hello-jni.cpp counter.cpp immutable.cpp argument.cpp class.cpp
LOCAL_LDLIBS += -llog

## enable rtti
LOCAL_CPP_FEATURES += rtti exceptions

## important!!
LOCAL_CPP_EXTENSION += .cpp

MY_SWIG_PACKAGE:=com.joker.test.hello_mk
MY_SWIG_INTERFACES:=Unix.i
MY_SWIG_TYPE:=cxx

include $(LOCAL_PATH)/my-swig-generate.mk

include $(BUILD_SHARED_LIBRARY)

