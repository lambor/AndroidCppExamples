//
// Created by lambor on 17-2-27.
//
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <unistd.h>

#include "examplelib.h"
#include "pthread.h"

extern "C"
{

struct NativeWorkerArgs
{
    jint id;
    jint iterations;
};

static jmethodID gOnNativeMessage = NULL;

static JavaVM *gVm = NULL;
static jobject gObj = NULL;
static pthread_mutex_t mutex;

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch07_Ch07Activity_nativeInit(JNIEnv *env, jobject instance) {

    env->GetJavaVM(&gVm);

    if(pthread_mutex_init(&mutex,NULL) != 0) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz,"Unable to initialize mutex");
        goto exit;
    }

    if (gObj == NULL) {
        gObj = env->NewGlobalRef(instance);
        if (gObj == NULL) {
            goto exit;
        }
    }

    if (gOnNativeMessage == NULL) {
        jclass clazz = env->GetObjectClass(instance);
        gOnNativeMessage = env->GetMethodID(clazz, "onNativeMessage", "(Ljava/lang/String;)V");
        if (gOnNativeMessage == NULL) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz, "Unable to find method onNativeMessage");
        }
    }

    exit:
    return;
}

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch07_Ch07Activity_nativeFree(JNIEnv *env, jobject instance) {
    if (gObj != NULL) {
        env->DeleteGlobalRef(gObj);
        gObj = NULL;
    }

    if (pthread_mutex_destroy(&mutex) != 0) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz,"Unable to destroy mutex");
    }
}

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch07_Ch07Activity_nativeWorker(JNIEnv *env, jobject instance,
                                                                      jint id, jint iterations) {
    for (jint i = 0; i < iterations; i++) {
        char message[26];
        sprintf(message, "worker %d: Iteration %d", id, i);
        jstring messageString = env->NewStringUTF(message);
        env->CallVoidMethod(instance, gOnNativeMessage, messageString);
        if (env->ExceptionOccurred() != NULL)
            break;
        sleep(1);
    }
}

static void* nativeWorkerThread(void* args)
{
    JNIEnv *env = NULL;
    if(gVm->AttachCurrentThread(&env,NULL) == 0) {
        NativeWorkerArgs* nativeWorkerArgs = (NativeWorkerArgs *) args;
        Java_com_joker_test_androidcppexamples_ch07_Ch07Activity_nativeWorker(env,gObj,
                                                                              nativeWorkerArgs->id,nativeWorkerArgs->iterations);
        delete nativeWorkerArgs;
        gVm->DetachCurrentThread();
    }

    return (void*) 1;
}

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch07_Ch07Activity_posixThread(JNIEnv *env, jobject instance,
                                                                     jint threads,
                                                                     jint iterations) {
    for(jint i=0;i<threads;i++) {
        NativeWorkerArgs* nativeWorkerArgs = new NativeWorkerArgs();
        nativeWorkerArgs->id = i;
        nativeWorkerArgs->iterations = iterations;

        pthread_t thread;
        int result = pthread_create(&thread,NULL,nativeWorkerThread,nativeWorkerArgs);
        if(result != 0) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz,"Unable to create thread");
        }
    }
}

static int giteration;

static void* mutexWorkerThread(void* args)
{
    JNIEnv *env = NULL;
    if(gVm->AttachCurrentThread(&env,NULL) == 0) {
        NativeWorkerArgs* nativeWorkerArgs = (NativeWorkerArgs *) args;

        while (true) {
            int _iteration = 0;
            if(pthread_mutex_lock(&mutex) != 0) {
                jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
                env->ThrowNew(exceptionClazz,"Unable to lock mutex");
                break;
            }
            giteration--;
            _iteration = giteration;
            if(pthread_mutex_unlock(&mutex) != 0) {
                jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
                env->ThrowNew(exceptionClazz, "Unable to unlock mutex");
                break;
            }
            if(_iteration<0) break;

            char message[26];
            sprintf(message, "worker %d: Iteration %d", nativeWorkerArgs->id, _iteration);
            jstring messageString = env->NewStringUTF(message);
            env->CallVoidMethod(gObj, gOnNativeMessage, messageString);
            if (env->ExceptionOccurred() != NULL)
                break;
            sleep(1);
        }

        delete nativeWorkerArgs;
        gVm->DetachCurrentThread();
    }

    return (void*) 1;
}

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch07_Ch07Activity_mutexThread(JNIEnv *env, jobject instance,
                                                                     jint threads, jint iteration) {

    giteration = iteration;
    for(jint i=0;i<threads;i++) {
        NativeWorkerArgs* nativeWorkerArgs = new NativeWorkerArgs();
        nativeWorkerArgs->id = i;

        pthread_t thread;
        int result = pthread_create(&thread,NULL,mutexWorkerThread,nativeWorkerArgs);
        if(result != 0) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz,"Unable to create thread");
        }
    }

}

//jint JNI_OnLoad(JavaVM *vm, void *reserved) {
//    gVm = vm;
//    return JNI_VERSION_1_1;
//}

}