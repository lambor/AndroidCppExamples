//
// Created by lambor on 17-2-9.
//
#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>

#include "examplelib.h"

#define TAG "ch03_7"

extern "C"
{

JavaVM *g_jvm = NULL;
jobject g_obj = NULL;

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_17_syncMethod_1JNI(JNIEnv *env, jobject instance) {
    env->MonitorEnter(instance);
    jclass clazz;
    clazz = env->GetObjectClass(instance);
    static jfieldID fieldId = NULL;
    if(fieldId == NULL)
        fieldId = env->GetFieldID(clazz,"count","I");
    jint count = env->GetIntField(instance,fieldId);
    count = count+1;
    env->SetIntField(instance,fieldId,count);
    LOGW("%d",count);
    env->DeleteLocalRef(clazz);
    env->MonitorExit(instance);
}


JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_17_setJNI_1ENV(JNIEnv *env, jobject instance) {
    env->GetJavaVM(&g_jvm);
    g_obj = env->NewGlobalRef(instance);
}

void *thread_func(void* arg)
{
    JNIEnv *env;

    if(g_jvm->AttachCurrentThread(&env,NULL)!= JNI_OK) {
        LOGE("%s: AttachCurrentThread() failed",__FUNCTION__);
        return NULL;
    }

    for(int i=0;i<500;i++)
        Java_com_joker_test_androidcppexamples_ch03_Ch03_17_syncMethod_1JNI(env,g_obj);

    if(g_jvm->DetachCurrentThread() != JNI_OK)
    {
        LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
    }
    pthread_exit(0);
}

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_17_startNativeThread(JNIEnv *env,
                                                                      jobject instance) {
    pthread_t pt;
    pthread_create(&pt,NULL,&thread_func,NULL);
}

}
