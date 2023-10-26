#ifndef STALKFUNCTIONS_H
#define STALKFUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "list.h"

#define MAXBUFF 1024
#define NTHREADS 4

extern struct sockaddr_in localSin, remoteSin;
extern char *HOSTNAME;
extern int socketDescriptor;
extern bool CHAT_ACTIVE;

void *keyInput();
void *sendData();
void *awaitUDP();
void *printScreen();
void init();
void joinThreads();
void freeVar();
void freeFnc();

#endif