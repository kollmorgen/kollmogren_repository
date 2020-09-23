#ifndef RGM6_H
#define RGM6_H

#ifdef __cplusplus
extern "C" {
#endif

/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifdef USE_XENO
#define eprintf(...)
#else
#define eprintf(...) printf (__VA_ARGS__)
#endif

#include "canfestival.h"
#include "Master.h"
#include "math.h"

/*
#define CAN_FIFO_LENGTH 100

#define DECLARE_A_CAN_FIFO \
static Message FIFO[CAN_FIFO_LENGTH];\
static int FIFO_First = 0;\
static int FIFO_Last = 0;\
\
static void PutInFIFO(Message *m)\
{\
	FIFO[FIFO_Last++] = *m;\
	FIFO_Last %= CAN_FIFO_LENGTH;\
}\
\
static void GetFromFIFO(Message *m)\
{\
	*m = FIFO[FIFO_First++];\
	FIFO_First %= CAN_FIFO_LENGTH;\
}\
\
static void TransmitMessage(CO_Data* d, UNS32 id)\
{\
	Message m;\
	GetFromFIFO(&m);\
	canDispatch(d, &m);\
}
*/
extern int display;

/**************************************************************************/
/*     定义物理数值:位置-度数(0-360);速度-度/s;安培 (MA)                      */
/**************************************************************************/


// extern int TargetPosition1_ph;	
// extern int TargetPosition2_ph;
// extern int TargetPosition3_ph;
// extern int TargetPosition4_ph;
// extern int TargetPosition5_ph;
// extern int TargetPosition6_ph;

// extern int ActualPosition1_ph;	
// extern int ActualPosition2_ph;
// extern int ActualPosition3_ph;
// extern int ActualPosition4_ph;
// extern int ActualPosition5_ph;
// extern int ActualPosition6_ph;

// extern int TargetVelocity1_ph;	
// extern int TargetVelocity2_ph;
// extern int TargetVelocity3_ph;
// extern int TargetVelocity4_ph;
// extern int TargetVelocity5_ph;
// extern int TargetVelocity6_ph;

// extern int ActualVelocity1_ph;
// extern int ActualVelocity2_ph;	
// extern int ActualVelocity3_ph;	
// extern int ActualVelocity4_ph;	
// extern int ActualVelocity5_ph;	
// extern int ActualVelocity6_ph;	

// extern int TargetTorque1_ph;
// extern int TargetTorque2_ph;
// extern int TargetTorque3_ph;
// extern int TargetTorque4_ph;		
// extern int TargetTorque5_ph;
// extern int TargetTorque6_ph;

// extern int ActualTorque1_ph;
// extern int ActualTorque2_ph;	
// extern int ActualTorque3_ph;
// extern int ActualTorque4_ph;
// extern int ActualTorque5_ph;
// extern int ActualTorque6_ph;

extern double TargetPosition_ph[7];
extern double ActualPosition_ph[7];
extern double TargetVelocity_ph[7];  
extern double ActualVelocity_ph[7]; 
extern double TargetTorque_ph[7];
extern double ActualTorque_ph[7];

extern double TargetPosition[7];
extern double TargetVelocity[7];  
extern double TargetTorque[7];
extern double group0_ph[7];
extern double group1_ph[7];
extern double group2_ph[7];
extern int group0[7];
extern int group1[7];
extern int group2[7];

void Rgm6_DiscoverNodes(void);

//[1-6]各个电机 [0]没有任务 [7]全部执行
extern int pos_control_invel;//基于速度反馈的位置控制

int Rgm6_Init();
int Rgm6_Start();
int Rgm6_Stop();
int shell_command();
int kollmorgen_mode_move();
int mode_move_1(int joint);
int mode_move_2(int joint,int mode);

int mode_move_4_position(int joint,double num);
int mode_move_4_velocity(int joint,double num);
int mode_move_4_current(int joint,double num);
int joint_1_move();


int kollmorgen_test_move();

int kollmorgen_move(int joint);

int move_stop(int joint);//joint=0的时候全部停止

int kollmorgen_help();
int kollmorgen_check();
void kollmorgen_teach();
int kollmorgen_teach_2();
int kollmorgen_display2(int joint,int num);

int kollmorgen_display(int joint,int num);
int kollmorgen_display_ph(int joint,int num);
/*
 * 注意有三种工作模式                            
 * 0-p-m-将"目标物理值"转化为"目标驱动值":位\速\力  
 * 1-m-p-将"实际驱动值"转化为"实际物理值":位\速\力  
 * 2-m-p-示教专用                              
 */
extern int kollmorgen_unit_conversion(int direction);
int v[7];
double Ff_com[7],v_err[7];
double t,p[7],pd[7],pdd[7],gra_compen[7];
int t_fourier_all;
int t_fourier_single;

void fourier_tra();
void fourier_tra_move();

// void fourier_tra_move5();
void fourier_tra_move4();
void fourier_tra_move3();
void fourier_tra_move2();
void fourier_tra_move1();
// void fourier_tra_move();
void fourier_tra_pos1(double t,double p[]);
void fourier_tra_pos2(double t,double p[]);
void fourier_tra_pos3(double t,double p[]);
void fourier_tra_pos4(double t,double p[]);
void fourier_tra_vel(double t,double pd[]);
void fourier_tra_acc(double t,double pdd[]);
void gra_com();

# define pi 3.141592653589793
void gravity_compensation(double p[],double gra_compen[]) ;

#ifdef __cplusplus
}
#endif

#endif // RGM6_H
