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

//Synchronization 
pthread_mutex_t syncLocalMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t syncLocalCond;
 
pthread_mutex_t syncRemoteMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t syncRemoteCond;

pthread_t threads[NTHREADS];

List* localMsgList;
List* remoteMsgList;

struct sockaddr_in localSin, remoteSin;
char *HOSTNAME;
int socketDescriptor;
bool CHAT_ACTIVE = true;
bool WAITING_TO_RECEIVE = true;
bool WAITING_TO_SEND = true;


void *keyboard(){
    char message[MAXBUFF];
    while(CHAT_ACTIVE){
            pthread_mutex_lock(&syncLocalMutex);
        while(WAITING_TO_SEND){
            fgets(message, MAXBUFF, stdin);
            printf("\033[32mYou: \033[0m%s\n", message);
            if(message[0] == '!'){
                printf("\033[0mTerminating chat....\n");
                CHAT_ACTIVE = false;
            }
            List_append(localMsgList, message);
            memset(message,'\0',MAXBUFF);
            WAITING_TO_SEND = false;
            pthread_mutex_unlock(&syncLocalMutex);
        }
        
    }
    pthread_exit(NULL);
}


void *screen(){
    while(CHAT_ACTIVE){
        pthread_mutex_lock(&syncRemoteMutex);
        while(!WAITING_TO_RECEIVE){
            while(List_count(remoteMsgList) > 0){
                List_first(remoteMsgList);
                printf("\033[34m%s: \033%s\n", HOSTNAME, (char*)List_remove(remoteMsgList));
            }
        }
        pthread_mutex_unlock(&syncRemoteMutex);
        WAITING_TO_RECEIVE = true;
    }
    pthread_exit(NULL);
}


void *send_msg(){
    char msg_content[MAXBUFF];
    while(CHAT_ACTIVE){
        pthread_mutex_lock(&syncLocalMutex);
        while(!WAITING_TO_SEND){
            while(List_count(localMsgList) > 0 && List_first(localMsgList) != NULL && List_curr(localMsgList) != NULL){
                List_first(localMsgList);
                strcpy(msg_content, (char*)List_remove(localMsgList));
                sendto(socketDescriptor, msg_content, MAXBUFF, 0, (struct sockaddr*)&remoteSin, sizeof(struct sockaddr_in));
            }
            pthread_mutex_unlock(&syncLocalMutex);
            WAITING_TO_SEND = true;
        }
    }
    pthread_exit(NULL);
}


void *receive(){
    char message[MAXBUFF];
    socklen_t remote_length = sizeof(remoteSin);
    while(CHAT_ACTIVE){
        pthread_mutex_lock(&syncRemoteMutex);
        while(WAITING_TO_RECEIVE){
            recvfrom(socketDescriptor, message, MAXBUFF, 0, (struct sockaddr*)&remoteSin, &remote_length);
            List_append(remoteMsgList, message);
            WAITING_TO_RECEIVE = false;
            pthread_mutex_unlock(&syncRemoteMutex);
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