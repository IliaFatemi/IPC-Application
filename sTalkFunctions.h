#ifndef STALKFUNCTIONS_H
#define STALKFUNCTIONS_H

#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXBUFF 1024
#define NTHREADS 4

extern struct sockaddr_in localSin, remoteSin;
extern char *HOSTNAME;
extern int socketDescriptor;
extern bool CHATTING;

void *keyboard();
void *screen();
void *send_msg();
void *receive();
void initializeThreads();

#endif