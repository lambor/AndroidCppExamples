//
// Created by lambor on 17-2-9.
//
#include <jni.h>
#include <string>
#include <android/log.h>

#include "examplelib.h"

#define TAG "ch03_5"

extern "C"
{
JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_15_accessMethods(JNIEnv *env, jobject instance) {

    jclass clazz;
    clazz = env->GetObjectClass(instance);
    jmethodID methodId;
    methodId = env->GetMethodID(clazz,"throwingMethod","()V");
    env->CallVoidMethod(instance,methodId);

    jthrowable ex;
    ex = env->ExceptionOccurred();
    if(0!=ex) {
        env->ExceptionClear();
        LOGI("null pointer exception occured.");
    }
}


JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_15_throwException(JNIEnv *env, jobject instance) {

    jclass _clazz;
    _clazz = env->FindClass("java/lang/NullPointerException");
    if(0!=_clazz) {
        env->ThrowNew(_clazz,"Exception Message.");
    }
}
}
