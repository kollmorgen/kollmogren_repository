#ifndef _KOLL_OPERATION_H
#define _KOLL_OPERATION_H


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
#include <time.h>
#include <signal.h>

extern pthread_mutex_t mut_tcp,mut_run,mut_j1_pos,mut_operation;
extern int thread_run_flag, thread_j1pos_flag,thread_operation_flag; 
extern pthread_t pt_tcp, pt_j1_pos, pt_run , pt_operation; 

extern double TargetPosition_ph[7];
extern double ActualPosition_ph[7];
extern double TargetVelocity_ph[7];  
extern double ActualVelocity_ph[7]; 
extern double TargetTorque_ph[7];
extern double ActualTorque_ph[7];

extern double TargetPosition[7];
extern double TargetVelocity[7];  
extern double TargetTorque[7];

#define operation_no_command 0
#define operation_stop   1


#define operation_finished   13
#define operation_not_finish 14
#define operation_current 100


int koll_operation_check(); 

int koll_operation_connect(int run_command);

void set_timer();/*默认定时器是1ms*/

void* f_koll_operation(void* args);

void koll_test_M3_run1();

void koll_test_M3_run2();

void koll_test_M3_run22();

void koll_test_M3_run3();

void koll_test_M3_run4();

int koll_test3_move_P();

void note_end();

void write_status(int traj);

void note_status(float t);

void operation_fourier_tra1(double t);

void operation_fourier_tra2(double t);

void operation_fourier_tra3(double t);

void operation_fourier_tra4(double t);

void koll_No_com();

void koll_Gra_com();

void koll_Gra_Ff_com();

void koll_test4_move();

void koll_test5_move();

void koll_test5_mark(int command);



#ifdef __cpluscplus
}
#endif

#endif

 