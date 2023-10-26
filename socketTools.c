#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <unistd.h>
#include "socketTools.h"
#include "sTalkFunctions.h"

void socketInitialization(){
    printf("Creating socket: ");
    SOCKET_FILE_DESCRIPTOR = socket(AF_INET, SOCK_DGRAM, UDP_PROTOCOL);

    if (SOCKET_FILE_DESCRIPTOR == -1){
        printf("\033[91mFailed to create socket\033[0m\n");
        exit(1);
    }
    printf("\033[92msocket created\033[0m\n");
}

void localSinInitialization(){
    //local socket setup
    LOCAL_SIN.sin_family = AF_INET;
    LOCAL_SIN.sin_port = htons(atoi(LOCAL_PORT));
    LOCAL_SIN.sin_addr.s_addr = htonl(INADDR_ANY);
}

void remoteSinInitialization(){
    //remote socket setup
    REMOTE_SIN.sin_family = AF_INET;
    REMOTE_SIN.sin_port = htons(atoi(REMOTE_PORT));
}

void bindInitialization(){
    printf("Binding: ");
    // int bind_ = bind(SOCKET_FILE_DESCRIPTOR, (struct sockaddr *)&localSin, sizeof(struct sockaddr_in));
    if(bind(SOCKET_FILE_DESCRIPTOR, (struct sockaddr *)&LOCAL_SIN, sizeof(struct sockaddr_in))){
        printf("\033[91mFailed to bind socket\033[0m\n");
        printf("Closing socket: \033[0m");
        close(SOCKET_FILE_DESCRIPTOR);
        printf("\033[91mSocket closed\n\n");
        exit(1);
    }
    printf("\033[92mBinding succesfull\033[0m\n");
}

void remoteHostFind(){
    printf("Looking for \033[1;94m%s\033[0m: ", REMOTE_HOSTNAME);
    
    REMOTE_HOST = gethostbyname(REMOTE_HOSTNAME);
    if(!REMOTE_HOST){
        printf("\033[91mFAILED TO FIND HOST\033[0m\n");
        printf("Closing socket: \033[0m");
        close(SOCKET_FILE_DESCRIPTOR);
        printf("\033[91mSocket closed\n\n");
        exit(1);
    }
    printf("\033[92mHost found\033[0m\n\n");

    ADDR_LIST = (struct in_addr**)REMOTE_HOST->h_addr_list;
    char* target;
    for(int i = 0; ADDR_LIST[i] != NULL; i++){
        target = inet_ntoa(*ADDR_LIST[i]);
        break;
    }
    if(inet_aton(target, &REMOTE_SIN.sin_addr) == 0){
        printf("inet_aton failed. \n");
        printf("Closing socket: \033[0m");
        close(SOCKET_FILE_DESCRIPTOR);
        printf("\033[91mSocket closed\n\n");
        exit(1);
    }
}

void localHostFind(){
    if (gethostname(LOCAL_HOSTNAME, sizeof(LOCAL_HOSTNAME)) != 0) {
        perror("\033[91mFAILED TO GET LOCAL HOST\033\n");
        exit(1);
    }
}

void printChatDetails(){
    printf("\n\033[96mChat Details:\033[0m");
    printf("\nLocal machine name: \033[92m%s\033[0m\n", LOCAL_HOSTNAME);
    printf("Local Port: \033[92m%s\033[0m\n", LOCAL_PORT);
    printf("Remote machine name: \033[1;94m%s\033[0m\n", REMOTE_HOSTNAME);
    printf("Remote Port: \033[1;94m%s\033[0m\n\n", REMOTE_PORT);
    printf("=================================================\n");
    printf("START Chatting\n\n");
}

void closeSocket(){
    closeSocket(SOCKET_FILE_DESCRIPTOR);
}