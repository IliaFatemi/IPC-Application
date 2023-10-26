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

List* localList;
List* remoteList;

struct sockaddr_in localSin, remoteSin;


int MAXBUFFER = 1024;
char *HOSTNAME;
int socketDescriptor;
bool CHAT_ACTIVE;

pthread_mutex_t localMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t remoteMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t localCond;
pthread_cond_t remoteCond;


pthread_t threadList[4]; // 4 threads

bool active = true;

bool emptySend = true;
bool emptyPrint = true;


void freeFnc(void *p){
    p = NULL;
}

void freeVar(){
    List_free(localList,freeFnc);
    List_free(remoteList,freeFnc);
    pthread_mutex_unlock(&localMutex);
    pthread_mutex_unlock(&remoteMutex);
    pthread_mutex_destroy(&localMutex);
    pthread_mutex_destroy(&remoteMutex);

}
// Add message to localList
void* keyInput(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    char msg[MAXBUFFER];
    while(active){
        pthread_testcancel();
        pthread_mutex_lock(&localMutex);
        while(!emptySend){
            pthread_testcancel();
            pthread_cond_wait(&localCond,&localMutex);
        } 
        while(emptySend){
            pthread_testcancel();
            fgets(msg,MAXBUFFER,stdin);
            if(msg[0] == 33 && strlen(msg) == 2){
                active=false;
                printf("You have ended the session.\n");
                
                List_append(localList,msg);
                emptySend = false;

                //Make sure ! is sent over
                pthread_mutex_unlock(&localMutex);
                pthread_cond_signal(&localCond);
                sleep(1); 
                pthread_cancel(threadList[1]);
                sleep(1);
                memset(msg,'\0',MAXBUFFER);
                close(socketDescriptor);

                freeVar();
                pthread_cancel(threadList[2]);
                pthread_cancel(threadList[3]);
                pthread_exit(0);
                return NULL;
            }else{
                List_append(localList,msg);
                emptySend = false;
                pthread_mutex_unlock(&localMutex);
                pthread_cond_signal(&localCond);
            }
        }
    }
    pthread_exit(NULL);
}

// Remove message from localList and send through socket
void* sendData(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    char temp[MAXBUFFER];
    bool exit = false;
    while(active){
        pthread_testcancel();
        pthread_mutex_lock(&localMutex);
        while(emptySend){
            pthread_testcancel();
            pthread_cond_wait(&localCond,&localMutex);
        }
        while(!emptySend){
            pthread_testcancel();
            while(List_count(localList) > 0 && List_first(localList) != NULL && List_curr(localList) != NULL) {
                pthread_testcancel();
                List_first(localList);
                strcpy(temp,(char*) List_remove(localList));
                sendto(socketDescriptor,temp,MAXBUFFER,0,(struct sockaddr *)&remoteSin, sizeof(struct sockaddr_in));
            }
            pthread_mutex_unlock(&localMutex);
            pthread_cond_signal(&localCond);
            emptySend = true;
        }
    }
    pthread_exit(NULL);
}

// Receive message from socket and add to remoteList
void* awaitUDP(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    char msg[MAXBUFFER];
    socklen_t fromlen = sizeof(remoteSin);
    while(active){
        // printf("Await UDP Start\n");
        pthread_testcancel();
        pthread_mutex_lock(&remoteMutex);
        while(!emptyPrint){
            pthread_testcancel();
            pthread_cond_wait(&remoteCond,&remoteMutex);
        }
        while(emptyPrint){
            pthread_testcancel();
            recvfrom(socketDescriptor,msg,MAXBUFFER,0,(struct sockaddr*)&remoteSin,&fromlen);
            if(msg[0] == 33 && strlen(msg) == 2) { 
                //33 ascii for !
                //2 b/c newline is counted 
                active = false;
                printf("Your partner has ended the session.\n");
                pthread_cancel(threadList[3]);
                memset(msg,'\0',MAXBUFFER);
                close(socketDescriptor);

                freeVar();

                pthread_cancel(threadList[0]);
                pthread_cancel(threadList[1]);

                pthread_exit(0);
            }else{
                List_append(remoteList,msg);
                emptyPrint = false;
                pthread_mutex_unlock(&remoteMutex);
                pthread_cond_signal(&remoteCond);
            }
        }
    }
    pthread_exit(NULL);
}
// Remove message from remoteList and print
void* printScreen(){
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    while(active){
        
        pthread_testcancel();
        pthread_mutex_lock(&remoteMutex);
        while(emptyPrint){
            pthread_testcancel();
            pthread_cond_wait(&remoteCond,&remoteMutex);
        }
        while(!emptyPrint){
            pthread_testcancel();
            while(List_count(remoteList) > 0){
                pthread_testcancel();
                List_first(remoteList);
                printf("Partner: %s",(char*)List_remove(remoteList));
            }
            pthread_mutex_unlock(&remoteMutex);
            pthread_cond_signal(&remoteCond);
            emptyPrint = true;
        }
    }
    pthread_exit(NULL);
}

void init(){
    localList = List_create();
    remoteList = List_create();
    pthread_create(&threadList[0],NULL,keyInput,NULL);
    pthread_create(&threadList[1],NULL,sendData,NULL);
    pthread_create(&threadList[2],NULL,awaitUDP,NULL);
    pthread_create(&threadList[3],NULL,printScreen,NULL);
}

void joinThreads(){
    for(int i = 0; i < 4; i ++){
        pthread_join(threadList[i],NULL);
    }
}