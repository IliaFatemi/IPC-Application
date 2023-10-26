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
        printf("Insufficient arguments give.\n");
        exit(1);
    }

    printf("Creating socket....\n");
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketDescriptor == -1){
        printf("Failed to create socket\n");
        exit(1);
    }
    printf("socket created\n\n");

    //local socket setup
    localSin.sin_family = AF_INET;
    localSin.sin_port = htons(atoi(argv[1]));
    localSin.sin_addr.s_addr = htonl(INADDR_ANY);

    //remote socket setup
    remoteSin.sin_family = AF_INET;
    remoteSin.sin_port = htons(atoi(argv[3]));

    printf("Binding...\n");
    if(bind(socketDescriptor, (struct sockaddr *)&localSin, sizeof(struct sockaddr_in)) == -1){
        printf("Failed to bind socket\n");
        exit(1);
    }
    printf("Binding succesfull\n\n");

    //search for host name
    printf("Looking for %s...\n", HOSTNAME);
    if(!gethostbyname(HOSTNAME)){
        printf("Could not find host\n");
        exit(1);
    }
    printf("Host found\n\n");

    init();
    joinThreads();
    
    close(socketDescriptor);


    // while(CHATTING){
    //     fgets(msg,MAXBUFF,stdin);

    //     printf("sendto\n");
    //     sendto(socketDescriptor, msg,MAXBUFF,0,(struct sockaddr *)&remoteSin, sizeof(struct sockaddr_in));
    //     socklen_t fromlen = sizeof(remoteSin);
    //     printf("receive\n");
    //     recvfrom(socketDescriptor,msg,MAXBUFF,0,(struct sockaddr*)&remoteSin,&fromlen);
    //     printf("Partner: %s",msg);
    // }

    return 0;
}