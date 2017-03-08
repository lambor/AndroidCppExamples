//
// Created by lambor on 17-3-8.
//

#include "echo_util.h"

void LogMessage(JNIEnv* env,jobject obj,const char *format, ...) {
    static jmethodID methodID = NULL;

    if(methodID == NULL) {
        jclass clazz = env->GetObjectClass(obj);
        methodID = env->GetMethodID(clazz,"logMessage","(Ljava/lang/String;)V");
        env->DeleteLocalRef(clazz);
    }

    if(methodID != NULL) {
        char buffer[MAX_LOG_MESSAGE_LENGTH];
        va_list ap;
        va_start(ap,format);
        vsnprintf(buffer,MAX_LOG_MESSAGE_LENGTH,format,ap);
        va_end(ap);

        jstring message = env->NewStringUTF(buffer);

        if(message != NULL) {
            env->CallVoidMethod(obj,methodID,message);
            env->DeleteLocalRef(message);
        }
    }
}

void ThrowException(JNIEnv *env,const char *className,const char *message) {
    jclass clazz = env->FindClass(className);
    if(clazz != NULL) {
        env->ThrowNew(clazz,message);
        env->DeleteLocalRef(clazz);
    }
}

void ThrowErrnoException(JNIEnv *env,const char *className,int errnum) {
    char buffer[MAX_LOG_MESSAGE_LENGTH];
    if(strerror_r(errnum,buffer,MAX_LOG_MESSAGE_LENGTH)!=-1) {
        strerror_r(errnum,buffer,MAX_LOG_MESSAGE_LENGTH);
    }
    ThrowException(env,className,buffer);
}

void LogAddress(JNIEnv *env,jobject obj,const char *message,const struct sockaddr_in *address) {
    char ip[INET_ADDRSTRLEN];
    if(inet_ntop(PF_INET,&(address->sin_addr),ip,INET_ADDRSTRLEN) == NULL) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    } else {
        unsigned short port = ntohs(address->sin_port);
        LogMessage(env,obj,"%s %s:%hu.",message,ip,port);
    }
}

void BindSocketToPort(JNIEnv *env,jobject obj,int sd, unsigned short port) {
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);
    LogMessage(env,obj,"Binding to port %hu.",port);
    if(bind(sd, (const sockaddr *) &address, sizeof(address)) == -1) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    }
}

unsigned short GetSocketPort(JNIEnv *env,jobject obj,int sd) {
    unsigned short port = 0;
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);

    if(getsockname(sd, (sockaddr *) &address, &addressLength)) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    } else {
        port = ntohs(address.sin_port);
        LogMessage(env,obj,"Binded to random port %hu.",port);
    }
    return port;
}