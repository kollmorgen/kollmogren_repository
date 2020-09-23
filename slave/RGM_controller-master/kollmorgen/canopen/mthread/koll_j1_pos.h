#ifndef _KOLL_J1_POS_H
#define _KOLL_J1_POS_H


#ifdef __cpluscplus
extern "C"{
#endif

#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>  
#include "math.h"


# define j1_open 1
# define j1_close 0

#define j1_pos_no_command 0
#define j1_pos_change 1
#define j1_pos_stop   2



extern pthread_mutex_t mut_tcp,mut_run,mut_j1_pos,mut_operation;
extern int thread_run_flag, thread_j1pos_flag,thread_operation_flag; 



extern double TargetPosition_ph[7];
extern double ActualPosition_ph[7];
extern double TargetVelocity_ph[7];  
extern double ActualVelocity_ph[7]; 
extern double TargetTorque_ph[7];
extern double ActualTorque_ph[7];

extern double TargetPosition[7];
extern double TargetVelocity[7];  
extern double TargetTorque[7];

extern pthread_t pt_tcp, pt_j1_pos, pt_run;


void* f_koll_j1_pos(void* args);// (1)thread_j1_pos: main

void j1_pos_run(float goal);

void j1_pos_PID_init();

int ji_pos_thread_connect();

void j1_pos_check();

extern float j1_kp, j1_kd, j1_ki;

#ifdef __cpluscplus
}
#endif

#endif
