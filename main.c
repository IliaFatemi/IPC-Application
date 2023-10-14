#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "s-talk.h"

struct sockaddr_in sinLocal, sinRemote;
int LOCAL_PORT, REMOTE_PORT;

int main(int arcv, char *argv[]){
    
    REMOTE_PORT = atoi(argv[1]);
    LOCAL_PORT = atoi(argv[2]);

    printf("%d %d\n", LOCAL_PORT, REMOTE_PORT);

    memset(&sinLocal, 0, sizeof(sinLocal));
    sinLocal.sin_family = AF_INET;
    sinLocal.sin_addr.s_addr = htonl(INADDR_ANY);
    sinLocal.sin_port = htons(LOCAL_PORT);

    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0); 

    if(bind(socketDescriptor, (struct sockaddr*) &sinLocal, sizeof(struct sockaddr_in)) == -1){
        printf("failed to create socket\n");
        exit(1);
    }

    // printf("%d %d\n", localPort, remotePort);

    keyboard();
    return 0;
}