#include <jni.h>
#include <string>
#include <android/log.h>

#include "examplelib.h"

#define TAG "ch03_4"


extern "C" {
jstring
Java_com_joker_test_androidcppexamples_ch03_Ch03_14_getHelloStr(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

/**
 * 如果使用GetStringUTFChars,最后需要ReleaseStringUTFChars
 * 估计因为Get使字符串引用加一,如果jni不释放对其引用,则JVM不能回收该字符串
 */
void Java_com_joker_test_androidcppexamples_ch03_Ch03_14_parseJavaStr(
        JNIEnv *env,
        jobject _this,
        jstring javaString) {
    const char* str;
    jboolean isCopy;
    str = env->GetStringUTFChars(javaString,&isCopy);

    if(0!=str) {
//        printf("Java string: %s",str);
        LOGI("Java string: %s",str);
        if(JNI_TRUE == isCopy) {
//            printf("C string is a copy of the Java string.");
            LOGI("C string is a copy of the Java string.");
        } else {
//            printf("C string points to actual string.");
            LOGI("C string points to actual string.");
        }
    }

    env->ReleaseStringUTFChars(javaString,str);
}

jintArray Java_com_joker_test_androidcppexamples_ch03_Ch03_14_getNumberArray(JNIEnv *env,
                                                                             jobject _this) {
    jintArray javaArray;
    javaArray = env->NewIntArray(10);
    if(0!=javaArray) {
//        env->DeleteLocalRef(javaArray);
        return javaArray;
    }
    else
        return 0;
}

void Java_com_joker_test_androidcppexamples_ch03_Ch03_14_initNumbersByRegion(JNIEnv *env,
                                                                     jobject _this,
                                                                      jintArray javaArray) {
    jint nativeArray[10];
    env->GetIntArrayRegion(javaArray,0,10,nativeArray);
    for(int i=0;i<10;i++) {
        nativeArray[i] = i;
    }
    env->SetIntArrayRegion(javaArray,0,10,nativeArray);
}

void Java_com_joker_test_androidcppexamples_ch03_Ch03_14_initNumbersDirectly(JNIEnv *env,
                                                                           jobject _this,
                                                                           jintArray javaArray) {
    jint* nativeDirectArray;
    jboolean isCopy;
    nativeDirectArray = env->GetIntArrayElements(javaArray,&isCopy);
    if(isCopy) {
        LOGI("native array is a copy of the Java array.");
    } else {
        LOGI("native array points to the Java array.");
    }
    for(int i=0;i<10;i++) {
        nativeDirectArray[i] = 10-i;
    }
    env->ReleaseIntArrayElements(javaArray,nativeDirectArray,0);
}

jstring Java_com_joker_test_androidcppexamples_ch03_Ch03_14_getInstanceField(JNIEnv *env,
                                                                             jobject _this) {
    jclass clazz;
    clazz = env->GetObjectClass(_this);
    jfieldID  instanceFieldId;
    instanceFieldId = env->GetFieldID(clazz,"instanceField","Ljava/lang/String;");
    jstring instanceField;
    instanceField = (jstring) env->GetObjectField(_this, instanceFieldId);
    return instanceField;
}

jstring Java_com_joker_test_androidcppexamples_ch03_Ch03_14_getStaticField(JNIEnv *env,
                                                                             jobject _this) {
    jclass clazz;
    clazz = env->GetObjectClass(_this);
    jfieldID  staticFieldId;
    staticFieldId = env->GetStaticFieldID(clazz,"staticField","Ljava/lang/String;");
    jstring staticField;
    staticField = (jstring) env->GetStaticObjectField(clazz, staticFieldId);
    return staticField;
}

JNIEXPORT jstring JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_14_invokeInstanceMethod(JNIEnv *env,
                                                                         jobject instance) {
    jclass clazz;
    clazz = env->GetObjectClass(instance);
    jmethodID instanceMethodId;
    instanceMethodId = env->GetMethodID(clazz,"instanceMethod","()Ljava/lang/String;");
    return (jstring) env->CallObjectMethod(instance, instanceMethodId);
}

JNIEXPORT jstring JNICALL
Java_com_joker_test_androidcppexamples_ch03_Ch03_14_invokeStaticMethod(JNIEnv *env,
                                                                       jobject instance) {
    // TODO
    jclass clazz;
    clazz = env->GetObjectClass(instance);
    jmethodID staticMethodId;
    staticMethodId = env->GetStaticMethodID(clazz,"staticMethod","()Ljava/lang/String;");
    return (jstring) env->CallStaticObjectMethod(clazz, staticMethodId);
}
}