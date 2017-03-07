//
// Created by lambor on 17-3-1.
//
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

static void LogMessage(JNIEnv* env,jobject obj,const char *format, ...) {
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

static void ThrowException(JNIEnv *env,const char *className,const char *message) {
    jclass clazz = env->FindClass(className);
    if(clazz != NULL) {
        env->ThrowNew(clazz,message);
        env->DeleteLocalRef(clazz);
    }
}

static void ThrowErrnoException(JNIEnv *env,const char *className,int errnum) {
    char buffer[MAX_LOG_MESSAGE_LENGTH];
    if(strerror_r(errnum,buffer,MAX_LOG_MESSAGE_LENGTH)!=-1) {
        strerror_r(errnum,buffer,MAX_LOG_MESSAGE_LENGTH);
    }
    ThrowException(env,className,buffer);
}

static int NewTcpSocket(JNIEnv *env,jobject obj) {
    LogMessage(env,obj,"Constructing a new TCP socket...");
    int tcpSocket = socket(PF_INET,SOCK_STREAM,0);
    if(-1 == tcpSocket) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    }
    return tcpSocket;
}

static void BindSocketToPort(JNIEnv *env,jobject obj,int sd, unsigned short port) {
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


static unsigned short GetSocketPort(JNIEnv *env,jobject obj,int sd) {
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

static void ListenOnSocket(JNIEnv *env,jobject obj,int sd,int backlog) {
    LogMessage(env,obj,"Listening on socket with a backlog of %d pending connections.",backlog);
    if(-1 == listen(sd,backlog)) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    }
}

static void LogAddress(JNIEnv *env,jobject obj,const char *message,const struct sockaddr_in *address) {
    char ip[INET_ADDRSTRLEN];
    if(inet_ntop(PF_INET,&(address->sin_addr),ip,INET_ADDRSTRLEN) == NULL) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    } else {
        unsigned short port = ntohs(address->sin_port);
        LogMessage(env,obj,"%s %s:%hu.",message,ip,port);
    }
}

static int AcceptOnSocket(JNIEnv *env,jobject obj,int sd) {
    struct sockaddr_in address;
    socklen_t  addressLength = sizeof(address);
    LogMessage(env,obj,"Waiting for a client connection...");
    int clientSocket = accept(sd,(struct sockaddr *)&address,&addressLength);
    if(clientSocket == -1) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    } else {
        LogAddress(env,obj,"Client connection from ",&address);
    }
    return clientSocket;
}

static ssize_t ReceiveFromSocket(JNIEnv *env,jobject obj,int sd,char *buffer,size_t bufferSize) {
    LogMessage(env,obj,"Receiving from the socket...");
    ssize_t recvSize = recv(sd,buffer,bufferSize-1,0);
    if(-1 == recvSize) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    }
    else
    {
        buffer[recvSize] = NULL;
        if(recvSize>0){
            LogMessage(env,obj,"Received %d bytes: %s",recvSize,buffer);
        }
        else
        {
            LogMessage(env,obj,"Client disconnected.");
        }
    }
    return recvSize;
}

static ssize_t SendToSocket(JNIEnv *env,jobject obj,int sd,const char *buffer,size_t bufferSize) {
    LogMessage(env,obj,"Sending to the socket...");
    ssize_t sentSize = send(sd,buffer,bufferSize,0);
    if(-1 == sentSize) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    }
    else
    {
        if(sentSize>0){
            LogMessage(env,obj,"Sent %d bytes: %s",sentSize,buffer);
        }
        else
        {
            LogMessage(env,obj,"Client disconnected.");
        }
    }
    return sentSize;
}

extern "C" {
JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch08_EchoServerActivity_nativeStartTcpServer(JNIEnv *env,
                                                                                    jobject instance,
                                                                                    jint port) {
    int serverSocket = NewTcpSocket(env, instance);
    if (env->ExceptionOccurred() == NULL) {
        BindSocketToPort(env, instance, serverSocket, (unsigned short) port);
        if (NULL != env->ExceptionOccurred()) {
            goto exit;
        }

        if (0 == port) {
            GetSocketPort(env, instance, serverSocket);
            if (env->ExceptionOccurred() != NULL) {
                goto exit;
            }
        }

        ListenOnSocket(env, instance, serverSocket, 4);
        if (env->ExceptionOccurred() != NULL) {
            goto exit;
        }

        int clientSocket = AcceptOnSocket(env, instance, serverSocket);
        if (env->ExceptionOccurred() != NULL) {
            goto exit;
        }

        char buffer[MAX_BUFFER_SIZE];
        ssize_t recvSize;
        ssize_t sentSize;

        while (1) {
            recvSize = ReceiveFromSocket(env, instance, clientSocket, buffer, MAX_BUFFER_SIZE);
            if ((0 == recvSize) || (env->ExceptionOccurred() != NULL))
                break;
            sentSize = SendToSocket(env, instance, clientSocket, buffer, recvSize);
            if ((0 == sentSize) || (env->ExceptionOccurred() != NULL)) {
                break;
            }
        }
        close(clientSocket);
    }

    exit:
    if (serverSocket > 0) {
        close(serverSocket);
    }
}

/**
 * Echo Client
 */
static void ConnectToAddress(JNIEnv *env, jobject obj, int sd, const char *ip, unsigned short port) {
    LogMessage(env,obj,"Connecting to %s:%uh...",ip,port);
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family = PF_INET;

    if(inet_aton(ip,&(address.sin_addr)) == 0) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    }
    else
    {
        address.sin_port = htons(port);
        if(connect(sd,(const sockaddr *)&address,sizeof(address)) == -1) {
            ThrowErrnoException(env,"java/io/IOException",errno);
        }
        else
        {
            LogMessage(env,obj,"Connected.");
        }
    }
}

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch08_EchoClientActivity_nativeStartTcpClient(JNIEnv *env,
                                                                                    jobject instance,
                                                                                    jstring ip_,
                                                                                    jint port,
                                                                                    jstring message_) {
    int clientSocket = NewTcpSocket(env,instance);

    /**
     * check NewTcpSocket result
     */
    if(env->ExceptionOccurred() == NULL) {
        const char *ip = env->GetStringUTFChars(ip_, 0);

        if(ip == NULL)
            goto exit;
        ConnectToAddress(env, instance, clientSocket, ip, (unsigned short) port);

        env->ReleaseStringUTFChars(ip_,ip);

        /**
         * check ConnectToAddress result
         */
        if(env->ExceptionOccurred() != NULL)
            goto exit;

        const char *message = env->GetStringUTFChars(message_, 0);

        if(message == NULL)
            goto exit;
        jsize messageSize = env->GetStringUTFLength(message_);
        SendToSocket(env,instance,clientSocket,message,messageSize);

        env->ReleaseStringUTFChars(message_,message);

        /**
         * check SendToSocket result
         */
        if(env->ExceptionOccurred() != NULL)
            goto exit;

        char buffer[MAX_BUFFER_SIZE];
        ReceiveFromSocket(env,instance,clientSocket,buffer,MAX_BUFFER_SIZE);

    }

    exit:
        if(clientSocket > -1) {
            close(clientSocket);
        }

}


}