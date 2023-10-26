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
    //==== Start Setup Socket connection ====
    socketDescriptor = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(socketDescriptor == -1) {
        printf("Socket creation failed.\n");
        exit(1);
    }
    int portLocal, portRemote;
    portLocal = atoi(argv[1]);
    portRemote = atoi(argv[3]);

    //Init local socket
    localSin.sin_family = AF_INET;
    localSin.sin_port = htons(portLocal);
    localSin.sin_addr.s_addr = htonl(INADDR_ANY);

    int b = bind(socketDescriptor, (struct sockaddr* )&localSin, sizeof(struct sockaddr_in));
    if(b == -1){
        printf("Socket bind failed.\n");
        exit(1);
    }
    //Init remote socket
    remoteSin.sin_family = AF_INET;
    remoteSin.sin_port = htons(portRemote);

    struct hostent *host;
    host = gethostbyname(argv[2]);
    if(!host) {
        printf("Host is not found. \n");
        exit(1);
    }

    struct in_addr** addrList = (struct in_addr**)host->h_addr_list;
    char* target;
    for(int i = 0; addrList[i] != NULL; i++){
        target = inet_ntoa(*addrList[i]);
        break;
    }
    if(inet_aton(target, &remoteSin.sin_addr) == 0){
        printf("inet_aton failed. \n");
        exit(1);
    }
    //==== End Setup Socket connection ====


    init();
    joinThreads();
    printf("Thank you for using s-talk!\n");
    close(socketDescriptor);
    return 0;
}