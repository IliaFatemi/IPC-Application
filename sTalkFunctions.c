#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "sTalkFunctions.h"
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <unistd.h>

char *HOSTNAME;

int socketDescriptor;

bool CHAT_ACTIVE = true;
bool WAITING_TO_RECEIVE = true;
bool TYPING_MSG = true;

struct sockaddr_in localSin, remoteSin;

//Synchronization 
pthread_mutex_t syncLocalMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t syncLocalCond;
 
pthread_mutex_t syncRemoteMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t syncRemoteCond;

pthread_t threads[NTHREADS];

List* localMsgList;
List* remoteMsgList;

socklen_t remoteSize = sizeof(remoteSin);


void *keyboard(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); // make it synched
    char message[MAXBUFF];
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncLocalMutex);
        for(;!TYPING_MSG;){
            pthread_testcancel();
            pthread_cond_wait(&syncLocalCond,&syncLocalMutex);
        } 
        for(;TYPING_MSG;){
            pthread_testcancel();
            fgets(message, MAXBUFF, stdin);
            if (message != ""){
                printf("\033[F\033[K");
            }
            printf("\033[31m\033[93mYou: \033[0m%s", message);
            if(chatEnded(message)){
                CHAT_ACTIVE = false;
                TYPING_MSG = false;
                printf("\033[91m\nYou terminated the session :(\n");
                List_append(localMsgList, message);
                pthread_mutex_unlock(&syncLocalMutex);
                pthread_cond_signal(&syncLocalCond);
                memset(message, NULLCHAR, MAXBUFF);
                close(socketDescriptor);
                deallocate();
                pthread_cancel(threads[1]);
                pthread_cancel(threads[2]);
                pthread_cancel(threads[3]);
                pthread_exit(0);
                return NULL;
            }else{
                List_append(localMsgList,message);
                TYPING_MSG = false;
                pthread_mutex_unlock(&syncLocalMutex);
                pthread_cond_signal(&syncLocalCond);
            }
        }
    }
    pthread_exit(NULL);
}


void *screen(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncRemoteMutex);
        while(WAITING_TO_RECEIVE){
            pthread_testcancel();
            pthread_cond_wait(&syncRemoteCond,&syncRemoteMutex);
        }
        while(!WAITING_TO_RECEIVE){
            pthread_testcancel();
            for(int i = 0; i < List_count(remoteMsgList); i++){
                pthread_testcancel();
                List_first(remoteMsgList);
                printf("\033[94m%s: \033[0m%s", HOSTNAME, (char*)List_remove(remoteMsgList));
            }
            pthread_mutex_unlock(&syncRemoteMutex);
            pthread_cond_signal(&syncRemoteCond);
            WAITING_TO_RECEIVE = true;
        }
    }
    pthread_exit(NULL);
}


void *send_msg(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    char send_message[MAXBUFF];
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncLocalMutex);
        while(TYPING_MSG){
            pthread_testcancel();
            pthread_cond_wait(&syncLocalCond,&syncLocalMutex);
        }
        while(!TYPING_MSG){
            pthread_testcancel();
            if(listNotEmpty()){
                for(int i = 0; i < List_count(localMsgList); i++){
                    pthread_testcancel();
                    List_first(localMsgList);
                    strcpy(send_message,(char*) List_remove(localMsgList));
                    sendto(socketDescriptor, send_message, MAXBUFF, 0, (struct sockaddr *) &remoteSin, sizeof(struct sockaddr_in));
                }
            }
            pthread_mutex_unlock(&syncLocalMutex);
            pthread_cond_signal(&syncLocalCond);
            TYPING_MSG = true;
        }
    }
    pthread_exit(NULL);
}


void *receive(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    char message[MAXBUFF];
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncRemoteMutex);
        while(!WAITING_TO_RECEIVE){
            pthread_testcancel();
            pthread_cond_wait(&syncRemoteCond, &syncRemoteMutex);
        }
        while(WAITING_TO_RECEIVE){
            pthread_testcancel();
            recvfrom(socketDescriptor, message, MAXBUFF, 0,(struct sockaddr*)&remoteSin, &remoteSize);
            if(chatEnded(message)) { 
                CHAT_ACTIVE = false;
                printf("\033[1;94m%s\033[91mhas ended the chat.\033[0m\n", HOSTNAME);
                pthread_cancel(threads[1]);
                memset(message, NULLCHAR, MAXBUFF);
                deallocate();
                close(socketDescriptor);
                pthread_cancel(threads[2]);
                pthread_cancel(threads[1]);
                pthread_exit(0);
            }else{
                List_append(remoteMsgList, message);
                WAITING_TO_RECEIVE = false;
                pthread_mutex_unlock(&syncRemoteMutex);
                pthread_cond_signal(&syncRemoteCond);
            }
        }
    }
    pthread_exit(NULL);
}


void initializeThreads(){
    localMsgList = List_create();
    remoteMsgList = List_create();

    pthread_create(&threads[0], NULL, keyboard, NULL);
    pthread_create(&threads[1], NULL, screen, NULL);
    pthread_create(&threads[2], NULL, send_msg, NULL);
    pthread_create(&threads[3], NULL, receive, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);
}

void deallocate(){
    List_free(localMsgList, NULL);
    List_free(remoteMsgList, NULL);
    pthread_mutex_unlock(&syncLocalMutex);
    pthread_mutex_unlock(&syncRemoteMutex);
    pthread_mutex_destroy(&syncLocalMutex);
    pthread_mutex_destroy(&syncRemoteMutex);
}

bool chatEnded(char message[]){
    return message[0] == '!';
}

bool listNotEmpty(){
    return List_first(localMsgList) != NULL && List_curr(localMsgList) != NULL && List_count(localMsgList) > 0;
}