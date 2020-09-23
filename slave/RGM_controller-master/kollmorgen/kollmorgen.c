#ifdef __cpluscplus
extern "C"{
#endif

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>  //互斥锁
#include <errno.h>
#include <signal.h>

#include "rgm6.h"
#include "rgmtcp.h"
#include "rgm_src.h"
#include "Fsensor.h"
#include "kollmorgen.h"
#include "koll_tcp.h"
#include "koll_run.h"
#include "koll_j1_pos.h"
#include "TestMaster.h"



#include<sys/types.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>


#ifdef __cpluscplus
}
#endif

#define SLEEP(time) sleep(time)


#define cst_str4(c1, c2, c3, c4) ((((unsigned int)0 | \
                                    (char)c4 << 8) | \
                                   (char)c3) << 8 | \
                                  (char)c2) << 8 | \
                                 (char)c1
#define QUIT 1


pthread_mutex_t mut_tcp,mut_run,mut_j1_pos,mut_operation;
int thread_run_flag; 
int thread_j1pos_flag;
int thread_tcp_flag;
int thread_operation_flag;
pthread_t pt_tcp, pt_j1_pos, pt_run , pt_operation; // three thread:(1)tcp,(2)position mode of joint1,(3)run


int main(void)
{
	// (1) 建立TCP通讯,tcp线程建立
	int ret1; 
	if (ret1 = tcp_connect() < 0){
		return -1;
	}
	// (2) CANopen初始化,CANopen线程建立
	int ret2; 
	if (ret2 = CANopen_connect() < 0){
		return -2;
	}
	// (3) 开启控制线程,run线程与ji_pos线程建立
    int ret3; 
	if (ret3 = run_connect() < 0){
		return -3;
	}
	// (4) 进入主循环等待tcp的结束信号
	system("clear");
	printf("等待TCP指令...\n");
	while(1){
		int ret4=1;
		pthread_mutex_lock(&mut_tcp);
        ret4=thread_tcp_flag;
        pthread_mutex_unlock(&mut_tcp);
		if (ret4 == Command_Shutdown){
			break;
		}
		//shell_command(); 
	}
	// (5) 结束CANopen线程
    int ret5; 
	if(ret5=Rgm6_Stop() != 0){
		printf("ERROR:Rgm6 fail to stop\n");
		return -4;
	}
	return 0;
}

int tcp_connect()
{
	thread_tcp_flag=1;
    int th_tcp   =pthread_create(&pt_tcp   ,NULL, f_koll_tcp, NULL); //开启TCP线程
	if (th_tcp != 0){
		printf("ERROR:fail to start TCP thread\n");
		return -2;
	}
	while(1){  // 等待连接结果
		int ret1=1;
		pthread_mutex_lock(&mut_tcp);
        ret1=thread_tcp_flag;
        pthread_mutex_unlock(&mut_tcp);
		if (ret1 == 0){
			return 0;
		} 
		else if (ret1 < 0){
			return -1;
		}
	}
}
int CANopen_connect()
{
	int retf=0;
	if ( retf = Rgm6_Init() < 0){
		printf("ERROR:Rgm6 fail to Initialization\n");
		return -1;
	}
	if (retf=Rgm6_Start() != 0){
		printf("ERROR:Rgm6 fail to start timerloop\n"); 
		return -2;
	}                   
    while(1){ //等待初始化结束
		EnterMutex();
		int canopnstatus = CANopen_status;
		LeaveMutex();
		if(canopnstatus == 1)
		return 0;
	}

}
int run_connect()
{
	int th_run   =pthread_create(&pt_run   ,NULL, f_koll_run, NULL);
	if (th_run != 0){
		printf("ERROR:fail to start run thread\n");
		return -1;
	}
	return 0;
}









