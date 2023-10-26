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

char *REMOTE_HOSTNAME, *REMOTE_PORT, *LOCAL_PORT, LOCAL_HOSTNAME[MAX_BUFF];

int SOCKET_FILE_DESCRIPTOR;

bool CHAT_ACTIVE = true;
bool WAITING_TO_RECEIVE = true;
bool TYPING_MSG = true;

struct sockaddr_in LOCAL_SIN, REMOTE_SIN;

struct hostent *REMOTE_HOST;

struct in_addr **ADDR_LIST;

//Synchronization 
pthread_mutex_t syncLocalMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t syncLocalCond;
 
pthread_mutex_t syncRemoteMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t syncRemoteCond;

pthread_t threads[N_THREADS];

List* localMsgList;
List* remoteMsgList;

socklen_t remoteSize = sizeof(REMOTE_SIN);

void *keyboard(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); // make it synched
    char message[MAX_BUFF];
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncLocalMutex);
        for(;!TYPING_MSG;){
            pthread_testcancel();
            pthread_cond_wait(&syncLocalCond,&syncLocalMutex);
        } 
        for(;TYPING_MSG;){
            pthread_testcancel();
            fgets(message, MAX_BUFF, stdin);
            if (message != ""){
                printf("\033[F\033[K");
            }
            printf("\033[31m\033[93mYou: \033[0m%s", message);
            if(chatEnded(message)){
                CHAT_ACTIVE = false;
                TYPING_MSG = false;
                WAITING_TO_RECEIVE = true;
                printf("\033[91m\nYou terminated the session :(\n");
                List_append(localMsgList, message);
                pthread_mutex_unlock(&syncLocalMutex);
                pthread_cond_signal(&syncLocalCond);
                pthread_cancel(threads[3]);
                memset(message, NULL_CHAR, MAX_BUFF);
                close(SOCKET_FILE_DESCRIPTOR);
                deallocate();
                pthread_cancel(threads[1]);
                pthread_cancel(threads[2]);
                pthread_exit(EXIT_STAT_SUCCESS);
                return NULL;
            }else{
                List_append(localMsgList,message);
                TYPING_MSG = false;
                pthread_mutex_unlock(&syncLocalMutex);
                pthread_cond_signal(&syncLocalCond);
            }
        }
    }
    pthread_exit(EXIT_STAT_UNKNOWN);
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
                printf("\033[94m%s: \033[0m%s", REMOTE_HOSTNAME, (char*)List_remove(remoteMsgList));
            }
            pthread_mutex_unlock(&syncRemoteMutex);
            pthread_cond_signal(&syncRemoteCond);
            WAITING_TO_RECEIVE = true;
        }
    }
    pthread_exit(EXIT_STAT_UNKNOWN);
}


void *send_msg(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    char send_message[MAX_BUFF];
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncLocalMutex);
        while(TYPING_MSG){
            pthread_testcancel();
            pthread_cond_wait(&syncLocalCond,&syncLocalMutex);
        }
        while(!TYPING_MSG){
            pthread_testcancel();
            if(!emptyList(localMsgList)){
                for(int i = 0; i < List_count(localMsgList); i++){
                    pthread_testcancel();
                    List_first(localMsgList);
                    strcpy(send_message,(char*) List_remove(localMsgList));
                    sendto(SOCKET_FILE_DESCRIPTOR, send_message, MAX_BUFF, MSG_DEFAULT_FLAG, (struct sockaddr *) &REMOTE_SIN, sizeof(struct sockaddr_in));
                }
            }
            pthread_mutex_unlock(&syncLocalMutex);
            pthread_cond_signal(&syncLocalCond);
            TYPING_MSG = true;
        }
    }
    pthread_exit(EXIT_STAT_UNKNOWN);
}


void *receive(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    char message[MAX_BUFF];
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncRemoteMutex);
        while(!WAITING_TO_RECEIVE){
            pthread_testcancel();
            pthread_cond_wait(&syncRemoteCond, &syncRemoteMutex);
        }
        while(WAITING_TO_RECEIVE){
            pthread_testcancel();
            recvfrom(SOCKET_FILE_DESCRIPTOR, message, MAX_BUFF, MSG_DEFAULT_FLAG, (struct sockaddr*)&REMOTE_SIN, &remoteSize);
            if(chatEnded(message)) { 
                CHAT_ACTIVE = false;
                printf("\033[1;94m%s\033[91m has ended the chat.\033[0m\n", REMOTE_HOSTNAME);
                pthread_cancel(threads[1]);
                memset(message, NULL_CHAR, MAX_BUFF);
                deallocate();
                close(SOCKET_FILE_DESCRIPTOR);
                pthread_cancel(threads[0]);
                pthread_cancel(threads[2]);
                pthread_exit(EXIT_STAT_SUCCESS);
            }else{
                List_append(remoteMsgList, message);
                WAITING_TO_RECEIVE = false;
                pthread_mutex_unlock(&syncRemoteMutex);
                pthread_cond_signal(&syncRemoteCond);
            }
        }
    }
    pthread_exit(EXIT_STAT_UNKNOWN);
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

bool emptyList(List *list){
    return List_first(list) == NULL && List_curr(list) == NULL && List_count(list) == 0;
}