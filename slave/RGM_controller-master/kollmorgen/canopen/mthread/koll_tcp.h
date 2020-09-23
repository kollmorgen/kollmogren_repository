
#ifndef _KOLL_TCP_H
#define _KOLL_TCP_H


#ifdef __cpluscplus
extern "C"{
#endif

#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <errno.h>
#include <pthread.h>  


#define KOLL_PORT            2040
#define KOLL_BUFFER_SIZE     8001
#define KOLL_SERVER_ADDR     "192.168.1.130"
#define KOLL_GET_ARRAY_LEN(array,len) {len = (sizeof(array) / sizeof(array[0]));}

typedef struct Koll_Receive_Command_struct {
    char c_index;                      //Command index
    char k_command[30];
    char data_status[20];
    float data_pos[6];
    float data_vel[6];
    float data_cur[6];
    float data_fsensor[6];
} S_Command ;

typedef struct Koll_Send_Command_struct {
    char c_index;                      //Command index
    char k_command[30];
    float motor_PID[3];
} R_Command;

extern double TargetPosition_ph[7];
extern double ActualPosition_ph[7];
extern double TargetVelocity_ph[7];  
extern double ActualVelocity_ph[7]; 
extern double TargetTorque_ph[7];
extern double ActualTorque_ph[7];

extern double TargetPosition[7];
extern double TargetVelocity[7];  
extern double TargetTorque[7];


extern int thread_tcp_flag;


void* f_koll_tcp(void* args);// (1)thread_tcp:main

int f_koll_tcp_init();// (2)thread_tcp:init

int f_koll_tcp_comm();// (3)thread_tcp:communication

int f_koll_tcp_stop();// (4)thread_tcp:stop
 
void Nocommand(void);

void init_r_command(R_Command * kollSstruct);

void init_s_command(S_Command * kollstruct);

void turn_to_run_thread(int command);

#ifdef __cpluscplus
}
#endif

#endif
