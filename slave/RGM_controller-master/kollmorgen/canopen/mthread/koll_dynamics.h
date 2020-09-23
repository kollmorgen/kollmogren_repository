#ifndef _KOLL_DYNAMICS_H
#define _KOLL_DYNAMICS_H


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
#include "math.h"

# define pi 3.141592653589793

extern double model_compen[7];

extern double TargetPosition_ph[7];
extern double ActualPosition_ph[7];
extern double TargetVelocity_ph[7];  
extern double ActualVelocity_ph[7]; 
extern double TargetTorque_ph[7];
extern double ActualTorque_ph[7];

extern double TargetPosition[7];
extern double TargetVelocity[7];  
extern double TargetTorque[7];

void koll_gravity_compensation(double p[7]);


#ifdef __cpluscplus
}
#endif

#endif