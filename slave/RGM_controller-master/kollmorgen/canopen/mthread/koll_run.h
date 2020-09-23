#ifndef _KOLL_RUN_H
#define _KOLL_RUN_H


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
#include <pthread.h>  


// 关节1控制指令
# define abso_pos 1
# define relative_pos 2 
# define j1_stop 3 

// operation指令

// 整体指令
# define No_Command    0
# define Command_Start    1
# define Command_stop     2
# define Command_Shutdown 3
# define Command_open_lock 4

// 调试指令
# define debug_j1_pos 11
# define debug_j1_para 12 

// 测试模式1：各个关节的速度模式-正传-反转
# define Test_M1_q1  21
# define Test_M1_q2  22
# define Test_M1_q3  23
# define Test_M1_q4  24
# define Test_M1_q5  25
# define Test_M1_q6  26
# define Test_M1_qq1 27
# define Test_M1_qq2 28
# define Test_M1_qq3 29
# define Test_M1_qq4 30
# define Test_M1_qq5 31
# define Test_M1_qq6 32
# define Test_M1_s1 33
# define Test_M1_s2 34
# define Test_M1_s3 35
# define Test_M1_s4 36
# define Test_M1_s5 37
# define Test_M1_s6 38

// 测试模式2：各个关节的：位置控制、速度控制、力矩控制
# define Test_M2_q1_P 41
# define Test_M2_q2_P 42
# define Test_M2_q3_P 43
# define Test_M2_q4_P 44
# define Test_M2_q5_P 45
# define Test_M2_q6_P 46

# define Test_M2_q1_V 51
# define Test_M2_q2_V 52
# define Test_M2_q3_V 53
# define Test_M2_q4_V 54
# define Test_M2_q5_V 55
# define Test_M2_q6_V 56

# define Test_M2_q1_T 61
# define Test_M2_q2_T 62
# define Test_M2_q3_T 63
# define Test_M2_q4_T 64
# define Test_M2_q5_T 65
# define Test_M2_q6_T 66

// 测试模式3：激励轨迹运行模式
# define Test_M3_run1 71
# define Test_M3_run2 72
# define Test_M3_run3 73
# define Test_M3_run4 74
# define Test_M3_run5 75

// 测试模式4：零力补偿模式
# define Test_M4_no 81
# define Test_M4_G  82
# define Test_M4_GF 83

// 测试模式5：拖动示教
# define Test_M5_pos_init     91
# define Test_M5_pos_identif  92
# define Test_M5_mark1 93
# define Test_M5_mark2 94
# define Test_M5_mark3 95
# define Test_M5_run_to_mark1 96
# define Test_M5_run_to_mark2 97
# define Test_M5_run_to_mark3 98


extern double test2_num;
extern pthread_mutex_t mut_tcp,mut_run,mut_j1_pos,mut_operation;
extern int thread_run_flag, thread_j1pos_flag,thread_operation_flag; 
extern double mark1_postion[7];
extern double mark2_postion[7];
extern double mark3_postion[7];

void* f_koll_run(void* args);// (1)thread_run: main

int koll_test_move(int joint);

int koll_stop();

int koll_open_the_lock();

int koll_rel_turn_10(int joint);


/* void j1_pos_move(int mode, double position)
   para @ mode : abso_pos,  relative_pos ,ji_stop
*/
void j1_pos_move(int mode, double position);

int koll_test2_move_V(int joint);

int koll_test2_move_P(int joint);




#ifdef __cpluscplus
}
#endif

#endif