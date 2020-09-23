#ifndef _KOLL_UNIT_CONVER_H
#define _KOLL_UNIT_CONVER_H


#ifdef __cpluscplus
extern "C"{
#endif

#include "canfestival.h"
#include "TestMaster.h"
#include <pthread.h>  
#include<stdio.h>

#define Nominal_to_Physical 1
#define Physical_to_Nominal 2

extern double TargetPosition_ph[7];
extern double ActualPosition_ph[7];
extern double TargetVelocity_ph[7];  
extern double ActualVelocity_ph[7]; 
extern double TargetTorque_ph[7];
extern double ActualTorque_ph[7];

extern double TargetPosition[7];
extern double TargetVelocity[7];  
extern double TargetTorque[7];


extern pthread_mutex_t mut_tcp,mut_run,mut_j1_pos,mut_operation;
extern int thread_run_flag, thread_j1pos_flag,thread_operation_flag; 

int koll_unit_conver(int direction,int joint);



#ifdef __cpluscplus
}
#endif

#endif