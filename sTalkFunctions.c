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
bool TYPING_MSG = true;


void *keyboard(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    char message[MAXBUFF];
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncLocalMutex);
        //wait to send message
        while(!TYPING_MSG){
            pthread_testcancel();
            pthread_cond_wait(&syncLocalCond,&syncLocalMutex);
        } 
        while(TYPING_MSG){
            pthread_testcancel();
            fgets(message, MAXBUFF, stdin);
            printf("\033[32mYou: \033[0m%s\n", message);
            if(message[0] == '!'){
                printf("\033[0mTerminating chat....\n");
                CHAT_ACTIVE = false;
            }
            List_append(localMsgList, message);
            memset(message,'\0',MAXBUFF);
            TYPING_MSG = false;
            pthread_mutex_unlock(&syncLocalMutex);
            pthread_cond_signal(&syncLocalCond);
        }
        
    }
    pthread_exit(NULL);
}


void *screen(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    while(CHAT_ACTIVE){
        
        pthread_testcancel();
        pthread_mutex_lock(&syncRemoteMutex);
        while(WAITING_TO_RECEIVE){
            pthread_testcancel();
            pthread_cond_wait(&syncRemoteCond,&syncRemoteMutex);
        }
        while(!WAITING_TO_RECEIVE){
            pthread_testcancel();
            while(List_count(remoteMsgList) > 0){
                pthread_testcancel();
                List_first(remoteMsgList);
                printf("Partner: %s",(char*)List_remove(remoteMsgList));
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
    char temp[MAXBUFF];
    bool exit = false;
    while(CHAT_ACTIVE){
        pthread_testcancel();
        pthread_mutex_lock(&syncLocalMutex);
        while(TYPING_MSG){
            pthread_testcancel();
            pthread_cond_wait(&syncLocalCond,&syncLocalMutex);
        }
        while(!TYPING_MSG){
            pthread_testcancel();
            while(List_count(localMsgList) > 0 && List_first(localMsgList) != NULL && List_curr(localMsgList) != NULL) {
                pthread_testcancel();
                List_first(localMsgList);
                strcpy(temp,(char*) List_remove(localMsgList));
                sendto(socketDescriptor,temp,MAXBUFF,0,(struct sockaddr *)&remoteSin, sizeof(struct sockaddr_in));
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
    char msg[MAXBUFF];
    socklen_t fromlen = sizeof(remoteSin);
    while(CHAT_ACTIVE){
        // printf("Await UDP Start\n");
        pthread_testcancel();
        pthread_mutex_lock(&syncRemoteMutex);
        while(!WAITING_TO_RECEIVE){
            pthread_testcancel();
            pthread_cond_wait(&syncRemoteCond,&syncRemoteMutex);
        }
        while(WAITING_TO_RECEIVE){
            pthread_testcancel();
            recvfrom(socketDescriptor,msg,MAXBUFF,0,(struct sockaddr*)&remoteSin,&fromlen);
            if(msg[0] == 33 && strlen(msg) == 2) { 
                //33 ascii for !
                //2 b/c newline is counted 
                CHAT_ACTIVE = false;
                printf("Your partner has ended the session.\n");
                pthread_cancel(threads[3]);
                memset(msg,'\0',MAXBUFF);
                close(socketDescriptor);

                // freeVar();

                pthread_cancel(threads[0]);
                pthread_cancel(threads[1]);

                pthread_exit(0);
            }else{
                List_append(remoteMsgList,msg);
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