#ifndef STALKFUNCTIONS_H
#define STALKFUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "list.h"

#define MAX_BUFF 1024
#define N_THREADS 4
#define NULL_CHAR '\0'
#define MSG_DEFAULT_FLAG 0
#define UDP_PROTOCOL 0
#define EXIT_STAT_SUCCESS 0
#define EXIT_STAT_UNKNOWN NULL

extern struct sockaddr_in LOCAL_SIN, REMOTE_SIN;

extern struct hostent *REMOTE_HOST;

extern struct in_addr **ADDR_LIST;

extern char *REMOTE_HOSTNAME, LOCAL_HOSTNAME[MAX_BUFF];
extern char *LOCAL_PORT, *REMOTE_PORT;

extern int SOCKET_FILE_DESCRIPTOR;

// Function prototypes for thread functions
void *keyboard();
void *screen();
void *send_msg();
void *receive();

// More function prototypes for initialization and cleanup
void initializeThreads();
void deallocate();
bool chatEnded(char message[]);
bool emptyList(List* list);

#endif