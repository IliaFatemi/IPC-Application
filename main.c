#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define MAXBUFF 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <local_port> <remote_machine_name> <remote_port>\n", argv[0]);
        exit(1);
    }

    int socketDescriptor;
    struct sockaddr_in remoteSin, localSin;
    socklen_t fromlen = sizeof(remoteSin);
    char *remoteHostName = argv[2];
    char *remotePort = argv[3];

    // Create a socket
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Setup local address
    localSin.sin_family = AF_INET;
    localSin.sin_port = htons(atoi(argv[1]));
    localSin.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind to the local address
    if (bind(socketDescriptor, (struct sockaddr *)&localSin, sizeof(localSin)) == -1) {
        perror("Failed to bind socket");
        exit(1);
    }

    // Get the remote host information
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(remoteHostName, remotePort, &hints, &result) != 0) {
        printf("Could not find host\n");
        exit(1);
    }

    // Setup remote address
    memcpy(&remoteSin, result->ai_addr, result->ai_addrlen);

    char message[MAXBUFF];

    while (1) {
        printf("Type a message: ");
        fgets(message, MAXBUFF, stdin);

        // Send the message to the remote host
        sendto(socketDescriptor, message, strlen(message), 0, (struct sockaddr *)&remoteSin, sizeof(remoteSin));

        // Receive a message from the remote host
        recvfrom(socketDescriptor, message, MAXBUFF, 0, (struct sockaddr *)&remoteSin, &fromlen);

        printf("Received: %s", message);
    }

    close(socketDescriptor);

    return 0;
}
