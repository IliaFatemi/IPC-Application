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

struct sockaddr_in localSin, remoteSin;
char *HOSTNAME;
int socketDescriptor;
bool CHATTING = true;

void *keyboard(){
    char message[MAXBUFF];
    while(CHATTING){
    pthread_mutex_lock(&syncLocalMutex);
        fgets(message, MAXBUFF, stdin);
        if(strlen(message) != 0){
            printf("%s\n", message);
            memset(message,'\0',MAXBUFF);
        }
            // CHATTING = false;
        
    }
    pthread_exit(NULL);
}


void *screen(){
    while(CHATTING){
        
    }
    pthread_exit(NULL);
}


void *send_msg(){
    while(CHATTING){
        
    }
    pthread_exit(NULL);
}


void *receive(){
    while(CHATTING){

    }
    pthread_exit(NULL);
}


void initializeThreads(){
    pthread_create(&threads[0], NULL, keyboard, NULL);
    pthread_create(&threads[1], NULL, screen, NULL);
    pthread_create(&threads[2], NULL, send_msg, NULL);
    pthread_create(&threads[3], NULL, receive, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);
}