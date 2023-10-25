#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "s-talk.h"
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <unistd.h>

struct sockaddr_in local, remote;

void keyboard(){
    printf("keyboard working\n");
}