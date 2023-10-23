#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_MSG_LEN 1024
#define END_CHAT_MARKER "!\n"

void communicate(int sockfd) {
    char message[MAX_MSG_LEN];
    
    while (1) {
        // Read message from user
        fgets(message, MAX_MSG_LEN, stdin);
        
        // Check if the message is the end marker
        if (strcmp(message, END_CHAT_MARKER) == 0) {
            printf("Chat ended.\n");
            break;
        }
        
        // Send message to the other user
        send(sockfd, message, strlen(message), 0);
        
        // Receive message from the other user
        int bytes_received = recv(sockfd, message, MAX_MSG_LEN, 0);
        if (bytes_received <= 0) {
            printf("Connection closed.\n");
            break;
        }
        
        // Null-terminate the received message
        message[bytes_received] = '\0';
        
        // Print the received message
        printf("Received: %s", message);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [my port number] [remote machine name] [remote port number]\n", argv[0]);
        exit(1);
    }
    
    int my_port = atoi(argv[1]);
    char* remote_machine = argv[2];
    int remote_port = atoi(argv[3]);
    
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(remote_port);
    server_addr.sin_addr.s_addr = inet_addr(remote_machine);

    // Connect to the remote machine
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to %s:%d\n", remote_machine, remote_port);

    // Start the chat
    communicate(sockfd);

    // Close the socket
    close(sockfd);

    return 0;
}
