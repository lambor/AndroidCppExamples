#include <jni.h>
#include <string>
extern "C"
JNIEXPORT jstring JNICALL
Java_com_joker_test_hello_1mk_Hello_stringFromJNI(JNIEnv *env, jclass type) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
