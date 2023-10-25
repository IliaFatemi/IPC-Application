#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

#define MAXBUFF 1024

int socketDescriptor;
struct sockaddr_in remoteSin;
socklen_t fromlen = sizeof(remoteSin);

void* receiveMessages(void* arg) {
    char message[MAXBUFF];
    while (1) {
        recvfrom(socketDescriptor, message, MAXBUFF, 0, (struct sockaddr *)&remoteSin, &fromlen);
        printf("Received: %s", message);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <local_port> <remote_machine_name> <remote_port>\n", argv[0]);
        exit(1);
    }

    char *remoteHostName = argv[2];
    char *remotePort = argv[3];

    // Create a socket
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Setup remote address
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(remoteHostName, remotePort, &hints, &result) != 0) {
        printf("Could not find host\n");
        exit(1);
    }
    memcpy(&remoteSin, result->ai_addr, result->ai_addrlen);

    pthread_t receiveThread;
    if (pthread_create(&receiveThread, NULL, receiveMessages, NULL) != 0) {
        perror("Failed to create receive thread");
        exit(1);
    }

    char message[MAXBUFF];
    while (1) {
        printf("Type a message: ");
        fgets(message, MAXBUFF, stdin);
        sendto(socketDescriptor, message, strlen(message), 0, (struct sockaddr *)&remoteSin, sizeof(remoteSin));
    }

    pthread_join(receiveThread, NULL);
    close(socketDescriptor);

    return 0;
}
