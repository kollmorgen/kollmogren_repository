#ifndef _KOLL_TCP_H
#define _KOLL_TCP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include <pthread.h>
#include <QTimer>

#define NUM_THREADS 2
#define TCPSAERVER_ADDR "192.168.1.130"


void* koll_send(void* args);
void* koll_recive(void* args);
int kolltcp_close();
int kolltcp_init();
int kolltcp_ping ();

extern int thread;
extern pthread_mutex_t mut,mut_rec;

#endif

