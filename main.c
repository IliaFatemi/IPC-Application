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
    initializeThreads();
    closeSocket();
    return 0;
}