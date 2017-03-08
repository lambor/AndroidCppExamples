//
// Created by lambor on 17-3-1.
//

#include "echo_util.h"

static int NewTcpSocket(JNIEnv *env,jobject obj) {
    LogMessage(env,obj,"Constructing a new TCP socket...");
    int tcpSocket = socket(PF_INET,SOCK_STREAM,0);
    if(-1 == tcpSocket) {
        ThrowErrnoException(env,"java/io/IOException",errno);
    }
    return tcpSocket;
}



static void ListenOnSocket(JNIEnv *env,jobject obj,int sd,int backlog) {
    LogMessage(env,obj,"Listening on socket with a backlog of %d pending connections.",backlog);
    if(-1 == listen(sd,backlog)) {
        ThrowErrnoException(env,"java/io/IOException",errno);
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
Java_com_joker_test_androidcppexamples_ch08_EchoTcpServerActivity_nativeStartTcpServer(JNIEnv *env,
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
Java_com_joker_test_androidcppexamples_ch08_EchoTcpClientActivity_nativeStartTcpClient(JNIEnv *env,
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
        SendToSocket(env, instance, clientSocket, message, (size_t) messageSize);

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