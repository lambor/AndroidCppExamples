//
// Created by lambor on 17-3-8.
//

#ifndef ANDROIDCPPEXAMPLES_ECHO_UTIL_H
#define ANDROIDCPPEXAMPLES_ECHO_UTIL_H

#include <jni.h>
#include <jni.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stddef.h>

#define MAX_LOG_MESSAGE_LENGTH 256
#define MAX_BUFFER_SIZE 80

void LogMessage(JNIEnv* env,jobject obj,const char *format, ...);
void ThrowException(JNIEnv *env,const char *className,const char *message);
void ThrowErrnoException(JNIEnv *env,const char *className,int errnum);
void LogAddress(JNIEnv *env,jobject obj,const char *message,const struct sockaddr_in *address);
void BindSocketToPort(JNIEnv *env,jobject obj,int sd, unsigned short port);
unsigned short GetSocketPort(JNIEnv *env,jobject obj,int sd);

#endif //ANDROIDCPPEXAMPLES_ECHO_UTIL_H