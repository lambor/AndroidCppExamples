//
// Created by lambor on 17-3-8.
//

#include "echo_util.h"

static int NewUdpSocket(JNIEnv* env,jobject obj) {
    LogMessage(env,obj,"Constructing a new UDP socket...");
    int udpSocket = socket(PF_INET,SOCK_DGRAM,0);

    if(udpSocket == -1) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    }
    return udpSocket;
}

static ssize_t ReceiveDatagramFromSocket(JNIEnv *env,jobject obj,int sd,struct sockaddr_in *address,char *buffer,size_t bufferSize) {
    socklen_t  addressLength = sizeof(struct sockaddr_in);
    LogMessage(env,obj,"Receiving from the socket...");
    ssize_t recvSize = recvfrom(sd,buffer,bufferSize,0,(struct sockaddr*)address,&addressLength);

    if(recvSize == -1) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    } else {
        LogAddress(env,obj,"Received from",address);
        buffer[recvSize] = NULL;
        if(recvSize > 0) {
            LogMessage(env,obj,"Received %d bytes: %s",recvSize,buffer);
        }
    }
    return recvSize;
}

static ssize_t SendDatagramToSocket(JNIEnv *env,jobject obj,int sd,const struct sockaddr_in* address,const char* buffer,size_t bufferSize) {
    LogAddress(env,obj,"Sending to",address);
    ssize_t sentSize = sendto(sd, buffer, bufferSize, 0, (const sockaddr *) address, sizeof(struct sockaddr_in));
    if(sentSize == -1) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    } else if(sentSize > 0) {
        LogMessage(env,obj,"Sent %d bytes: %s",sentSize,buffer);
    }
    return sentSize;
}

extern "C" {
JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch09_EchoUdpServerActivity_nativeStartUdpServer(JNIEnv *env,
                                                                                    jobject instance,
                                                                                    jint port) {
    int serverSocket = NewUdpSocket(env,instance);
    if(env->ExceptionOccurred() == NULL) {
        BindSocketToPort(env, instance, serverSocket, (unsigned short) port);
        if(env->ExceptionOccurred() != NULL)
            goto exit;
        if(port == 0) {
            GetSocketPort(env,instance,serverSocket);
            if(env->ExceptionOccurred() != NULL)
                goto exit;
        }

        struct sockaddr_in address;
        memset(&address,0,sizeof(address));

        char buffer[MAX_BUFFER_SIZE];
        ssize_t recvSize;
        ssize_t sentSize;

        recvSize = ReceiveDatagramFromSocket(env,instance,serverSocket,&address,buffer,MAX_BUFFER_SIZE);
        if((recvSize ==0) || env->ExceptionOccurred()!=NULL) {
            goto exit;
        }

        sentSize = SendDatagramToSocket(env,instance,serverSocket,&address,buffer,(size_t)recvSize);
    }

    exit:
        if(serverSocket > 0) {
            close(serverSocket);
        }
}

JNIEXPORT void JNICALL
Java_com_joker_test_androidcppexamples_ch09_EchoUdpClientActivity_nativeStartUdpClient(JNIEnv *env,
                                                                                       jobject instance,
                                                                                       jstring ip_,
                                                                                       jint port,
                                                                                       jstring message_) {
    int clientSocket = NewUdpSocket(env,instance);
    if(env->ExceptionOccurred() == NULL) {
        struct sockaddr_in address;
        memset(&address,0, sizeof(address));
        address.sin_family = PF_INET;

        const char *ip = env->GetStringUTFChars(ip_, 0);
        if(ip == NULL) goto exit;

        int result = inet_aton(ip,&(address.sin_addr));
        env->ReleaseStringUTFChars(ip_, ip);

        if(result == 0) {
            ThrowErrnoException(env,"java/io/IOException",errno);
            goto exit;
        }
        address.sin_port = htons(port);

        const char *message = env->GetStringUTFChars(message_, 0);
        if(message == NULL) goto exit;

        jsize messageSize = env->GetStringUTFLength(message_);
        SendDatagramToSocket(env,instance,clientSocket,&address,message,messageSize);
        env->ReleaseStringUTFChars(message_, message);

        if(env->ExceptionOccurred() != NULL) {
            goto exit;
        }

        char buffer[MAX_BUFFER_SIZE];
        memset(&address,0, sizeof(address));
        ReceiveDatagramFromSocket(env,instance,clientSocket,&address,buffer,MAX_BUFFER_SIZE);
    }

    exit:
        if(clientSocket > 0)
            close(clientSocket);
}
}