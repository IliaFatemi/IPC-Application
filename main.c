#include "sTalkFunctions.h"
#include "socketTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    /*
    argv[1] = local machine port
    argv[2] = remote machine name
    argv[3] = remote machine port
    */

   
    LOCAL_PORT = argv[1];
    REMOTE_HOSTNAME = argv[2];
    REMOTE_PORT = argv[3];

    if (argc != 4){
        printf("\033[91mInsufficient arguments give.\033[0m\n");
        exit(1);
    }

    socketInitialization();
    localSinInitialization();
    localSinInitialization();
    remoteSinInitialization();
    bindInitialization();
    remoteHostFind();
    localHostFind();
    printChatDetails();

    // printf("Creating socket: ");
    // socketDescriptor = socket(AF_INET, SOCK_DGRAM, UDP_PROTOCOL);

    // if (socketDescriptor == -1){
    //     printf("\033[91mFailed to create socket\033[0m\n");
    //     exit(1);
    // }
    // printf("\033[92msocket created\033[0m\n");

    //local socket setup
    // localSin.sin_family = AF_INET;
    // localSin.sin_port = htons(atoi(LOCAL_PORT));
    // localSin.sin_addr.s_addr = htonl(INADDR_ANY);

    // //remote socket setup
    // remoteSin.sin_family = AF_INET;
    // remoteSin.sin_port = htons(atoi(REMOTE_PORT));

    // printf("Binding: ");
    // // int bind_ = bind(socketDescriptor, (struct sockaddr *)&localSin, sizeof(struct sockaddr_in));
    // if(bind(socketDescriptor, (struct sockaddr *)&localSin, sizeof(struct sockaddr_in))){
    //     printf("\033[91mFailed to bind socket\033[0m\n");
    //     printf("Closing socket: \033[0m");
    //     close(socketDescriptor);
    //     printf("\033[91mSocket closed\n\n");
    //     exit(1);
    // }
    // printf("\033[92mBinding succesfull\033[0m\n");

    //search for host name
    // printf("Looking for \033[1;94m%s\033[0m: ", REMOTE_HOSTNAME);
    
    // host = gethostbyname(REMOTE_HOSTNAME);
    // if(!host){
    //     printf("\033[91mFAILED TO FIND HOST\033[0m\n");
    //     printf("Closing socket: \033[0m");
    //     close(socketDescriptor);
    //     printf("\033[91mSocket closed\n\n");
    //     exit(1);
    // }
    // printf("\033[92mHost found\033[0m\n\n");

    // addrList = (struct in_addr**)host->h_addr_list;
    // char* target;
    // for(int i = 0; addrList[i] != NULL; i++){
    //     target = inet_ntoa(*addrList[i]);
    //     break;
    // }
    // if(inet_aton(target, &remoteSin.sin_addr) == 0){
    //     printf("inet_aton failed. \n");
    //     printf("Closing socket: \033[0m");
    //     close(socketDescriptor);
    //     printf("\033[91mSocket closed\n\n");
    //     exit(1);
    // }

    // if (gethostname(LOCAL_HOSTNAME, sizeof(LOCAL_HOSTNAME)) != 0) {
    //     perror("\033[91mFAILED TO GET LOCAL HOST\033\n");
    //     exit(1);
    // }
    
    // printf("\n\033[96mChat Details:\033[0m");
    // printf("\nLocal machine name: \033[92m%s\033[0m\n", LOCAL_HOSTNAME);
    // printf("Local Port: \033[92m%s\033[0m\n", LOCAL_PORT);
    // printf("Remote machine name: \033[1;94m%s\033[0m\n", REMOTE_HOSTNAME);
    // printf("Remote Port: \033[1;94m%s\033[0m\n\n", REMOTE_PORT);

    // char buffer[1024];
    // struct sockaddr_in senderAddress;
    // socklen_t addrLen = sizeof(senderAddress);

    // // printf("Waiting %s to connect....\n", REMOTE_HOSTNAME);
    // while (1) {
    //     ssize_t bytesReceived = recvfrom(socketDescriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddress, &addrLen);

    //     if (bytesReceived > 0) {
    //         printf("\033[1;34m%s \033[0mhas connected: %s\n", REMOTE_HOSTNAME, buffer);
    //         break;
    //     }w
    // }

    // printf("=================================================\n");
    // printf("START Chatting\n\n");

    initializeThreads();
    closeSocket();
    return 0;
}