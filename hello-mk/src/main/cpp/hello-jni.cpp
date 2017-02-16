#include <jni.h>
#include <string>
#include <android/log.h>
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))
#define TAG "hello"
extern "C"
{
JNIEXPORT jstring JNICALL
Java_com_joker_test_hello_1mk_Hello_stringFromJNI(JNIEnv *env, jclass type) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

}
