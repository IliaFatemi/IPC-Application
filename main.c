#include "sTalkFunctions.h"
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
    argv[1] = local port
    argv[2] = remote machine name
    argv[3] = remote port
    */
   
    HOSTNAME = argv[2];

    if (argc != 4){
        printf("\033[91mInsufficient arguments give.\033[0m\n");
        exit(1);
    }

    printf("Creating socket: ");
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketDescriptor == -1){
        printf("\033[91mFailed to create socket\033[0m\n");
        exit(1);
    }
    printf("\033[92msocket created\033[0m\n");

    //local socket setup
    localSin.sin_family = AF_INET;
    localSin.sin_port = htons(atoi(argv[1]));
    localSin.sin_addr.s_addr = htonl(INADDR_ANY);

    //remote socket setup
    remoteSin.sin_family = AF_INET;
    remoteSin.sin_port = htons(atoi(argv[3]));

    printf("Binding: ");
    // int bind_ = bind(socketDescriptor, (struct sockaddr *)&localSin, sizeof(struct sockaddr_in));
    if(bind(socketDescriptor, (struct sockaddr *)&localSin, sizeof(struct sockaddr_in))){
        printf("\033[91mFailed to bind socket\033[0m\n");
        printf("Closing socket: \033[0m");
        close(socketDescriptor);
        printf("\033[91mSocket closed\n\n");
        exit(1);
    }
    printf("\033[92mBinding succesfull\033[0m\n");

    //search for host name
    printf("Looking for \033[1;94m%s\033[0m: ", HOSTNAME);
    struct hostent *host;
    host = gethostbyname(HOSTNAME);
    if(!host){
        printf("\033[91mFAILED TO FIND HOST\033[0m\n");
        printf("Closing socket: \033[0m");
        close(socketDescriptor);
        printf("\033[91mSocket closed\n\n");
        exit(1);
    }
    printf("\033[92mHost found\033[0m\n\n");

    struct in_addr** addrList = (struct in_addr**)host->h_addr_list;
    char* target;
    for(int i = 0; addrList[i] != NULL; i++){
        target = inet_ntoa(*addrList[i]);
        break;
    }
    if(inet_aton(target, &remoteSin.sin_addr) == 0){
        printf("inet_aton failed. \n");
        printf("Closing socket: \033[0m");
        close(socketDescriptor);
        printf("\033[91mSocket closed\n\n");
        exit(1);
    }

    // char buffer[1024];
    // struct sockaddr_in senderAddress;
    // socklen_t addrLen = sizeof(senderAddress);

    // // printf("Waiting %s to connect....\n", HOSTNAME);
    // while (1) {
    //     ssize_t bytesReceived = recvfrom(socketDescriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderAddress, &addrLen);

    //     if (bytesReceived > 0) {
    //         printf("\033[1;34m%s \033[0mhas connected: %s\n", HOSTNAME, buffer);
    //         break;
    //     }w
    // }

    printf("=================================================\n");
    printf("START Chatting\n\n");

    initializeThreads();
    printf("Closing socket: \033[0m");
    close(socketDescriptor);
    printf("\033[92mSuccesfull\033[0m\n\n");
    return 0;
}