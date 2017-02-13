//
// Created by lambor on 17-2-9.
//
#include <jni.h>
#include <string>
#include <android/log.h>

#include "examplelib.h"

#define TAG "ch03_6"

extern "C"
{
JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_16_testLocalRef(JNIEnv *env, jobject instance) {

    jclass clazz;
    clazz = env->GetObjectClass(instance);
    //手动删除局部引用,是为了腾出局部引用槽.因为局部引用的最大数量是一定的
    //env->DeleteLocalRef(clazz);
}

jintArray g_IntArr;

JNIEXPORT jintArray JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_16_makeGlobalRef(JNIEnv *env, jobject instance) {

    jintArray intArr;
    intArr = env->NewIntArray(10);
    g_IntArr = (jintArray) env->NewGlobalRef(intArr);
    env->DeleteLocalRef(intArr);
    return g_IntArr;
}

/**
 * !!!!!!!
 * global ref 全局引用不能从JAVA中传进来,因为这两个值是不同的.
 * 即 Java 中 获取到的makeGlobalRef返回的引用不是全局引用.而是全局引用指向的对象的另一个局部引用
 */
JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_16_releaseGlobalRef(JNIEnv *env, jobject instance) {
    env->DeleteGlobalRef(g_IntArr);
}



jintArray g_WeakIntArr;

JNIEXPORT jintArray JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_16_makeWeakRef(JNIEnv *env, jobject instance) {
    jintArray intArr;
    intArr = env->NewIntArray(10);
    g_WeakIntArr = (jintArray) env->NewWeakGlobalRef(intArr);
    env->DeleteLocalRef(intArr);
    return g_WeakIntArr;
}

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_16_releaseWeakRef(JNIEnv *env, jobject instance) {
    if(JNI_FALSE == env->IsSameObject(g_WeakIntArr,NULL)) {
        env->DeleteWeakGlobalRef(g_WeakIntArr);
    }
}
}
