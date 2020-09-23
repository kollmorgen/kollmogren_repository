#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "canfestival.h"
//#include <can_driver.h>
//#include <timers_driver.h>
#include "sdo.h"
#include "rgm6.h"
#include "Master.h"
#include "math.h"
#include "unistd.h"
#include "Fsensor.h"
int pidh;
int pidcount;
int timecount;

int pos_control_invel=0;
CO_Data* CANOpenShellOD_Data;


double group0_ph[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double group1_ph[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double group2_ph[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};

int group0[7]={0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int group1[7]={0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int group2[7]={0x0,0x0,0x0,0x0,0x0,0x0,0x0};


s_BOARD MasterBoard = {"0", "1M"};

void InitNodes(CO_Data* d, UNS32 id)
{
	if(MasterBoard.baudrate){
		printf("************ InitNodes ******************\n");
		/* Defining the node Id */
		setNodeId(&TestMaster_Data, 0x01);
		/* init */
		setState(&TestMaster_Data, Initialisation);
	}
}

void Exit(CO_Data* d, UNS32 id)
{
	if(strcmp(MasterBoard.baudrate, "none")){
		printf("************ Exit ******************\n");
		masterSendNMTstateChange(&TestMaster_Data, 0x00, NMT_Reset_Node);    
       	//Stop master
		setState(&TestMaster_Data, Stopped);
	}
}

/**
  1-定义:canopen的初始化定义
  2-参数:无参数
  3-返回:
*/
int Rgm6_Init()
{
	/*注释的原因:*/
    // printf("******************************\n");
    // char* LibraryPath="/home/root/libcanfestival_can_socket.so";
	// if (LoadCanDriver(LibraryPath) == NULL)
	// {
	//     printf("Unable to load library: %s\n",LibraryPath);
	// 	return -1;
	// }
	ControlWord = 0x04;
	OperationMode1 = 0x03;
	OperationMode2 = 0x03;
	OperationMode3 = 0x03;
	OperationMode4 = 0x03;
	OperationMode5 = 0x03;
	OperationMode6 = 0x03;
	  printf("hello i am here :8.16-2\n");

	TimerInit();//开启了一个线程的定时器功能
	  printf("hello i am here :8.16-3\n");



	if(strcmp(MasterBoard.baudrate, "none"))
	{	
		//CANOpen的网络管理使用了master/slave结构。
		//Master通过模块控制服务，可以控制slave的状态：{STOPPED, PRE-OPERATIONAL, OPERATIONAL, INITIALISING}.四个状态;
		//模块控制服务可以只针对一个节点，也可以是所有节点同时改变。
	    //设备上电初始化之后自动执行initialsing，之后设备进入PRE-OPERATIONAL状态，此时SDO服务开启(在这个状态中完成参数配置).
		//然后节点可以转换到OPERATIONAL状态，在OPERATIONAL状态PDO服务开启。


		TestMaster_Data.heartbeatError = TestMaster_heartbeatError; 
		TestMaster_Data.initialisation = TestMaster_initialisation;
		TestMaster_Data.preOperational = TestMaster_preOperational;//驱动器boot函数
		TestMaster_Data.operational = TestMaster_operational; 
		TestMaster_Data.stopped = TestMaster_stopped;
		TestMaster_Data.post_sync = TestMaster_post_sync;            //sync
		TestMaster_Data.post_TPDO = TestMaster_post_TPDO;            //tpdo
		TestMaster_Data.post_emcy = TestMaster_post_emcy;            //emcy
		TestMaster_Data.post_SlaveBootup=TestMaster_post_SlaveBootup;//驱动器全部反馈boot之后就会触发发动master
	
		if(!canOpen(&MasterBoard,&TestMaster_Data))
		{
			eprintf("Cannot open Master Board (%s,%s)\n",MasterBoard.busname, MasterBoard.baudrate);
			return -2;
		}
	}	

	return 0;
}
int shell_command()
{

	    EnterMutex();
	    OperationMode1 = 0x03;
	    OperationMode2 = 0x03;
	    OperationMode3 = 0x03;
	    OperationMode4 = 0x03;
	    OperationMode5 = 0x03;
	    OperationMode6 = 0x03;
		TargetVelocity_ph[1]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(1);
		TargetVelocity_ph[2]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(2);
		TargetVelocity_ph[3]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(3);
		TargetVelocity_ph[4]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(4);
		TargetVelocity_ph[5]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(5);
		TargetVelocity_ph[6]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(6);
		LeaveMutex();
		char command[200];
		
		system("clear");
		printf("*********************\n");
		printf("* 欢迎进入机械臂测试*\n");
		printf("*   1-测试(test)    *\n");
		printf("*   2-实验(mode)    *\n");
		printf("*   3-检测(check)   *\n");
		printf("*   4-示教(teach)   *\n");
		printf("*   5-帮助(help)    *\n");
		printf("*   6-激励轨迹(tra)    *\n");
		printf("*   7-重力摩擦力补偿(gra) *\n");
		printf("*********************\n");
		printf("请选择测试模式:");
			    fgets(command, sizeof(command), stdin);
		     if(strncmp(command,"test",4)==0 || strncmp(command,"1",1)==0 || strncmp(command,"t",1)==0){kollmorgen_test_move();}
		else if(strncmp(command,"mode",4)==0 || strncmp(command,"2",1)==0 || strncmp(command,"m",1)==0){kollmorgen_mode_move();}
		else if(strncmp(command,"check",5)==0|| strncmp(command,"3",1)==0 || strncmp(command,"c",1)==0){kollmorgen_check();}
		else if(strncmp(command,"help",4)==0 || strncmp(command,"5",1)==0 || strncmp(command,"h",1)==0){kollmorgen_help();}
		else if(strncmp(command,"teach",5)==0 || strncmp(command,"4",1)==0 || strncmp(command,"a",2)==0){kollmorgen_teach();}
        else if(strncmp(command,"tra",3)==0 || strncmp(command,"6",1)==0 || strncmp(command,"a",2)==0){fourier_tra();}
        else if(strncmp(command,"gra",3)==0 || strncmp(command,"7",1)==0 || strncmp(command,"g",2)==0){gra_com();}
		else{printf("指令错误\n");return 0;}
}
void gra_com(){
	
	char command[200];
	EnterMutex();
	OperationMode1 = 0x04;
	OperationMode2 = 0x04;
	OperationMode3 = 0x04;
	OperationMode4 = 0x04;
	OperationMode5 = 0x04;
	OperationMode6 = 0x04;
	LeaveMutex();
	system("clear");
	printf("*********************\n");
	printf("* 重力摩擦力补偿实验 *\n");
	printf("*   1-数据检测(check)    *\n");
	printf("*********************\n");
	printf("请选择测试模式:");
	fgets(command, sizeof(command), stdin);
		 if(strncmp(command,"exit",4)==0 || strncmp(command,"e",1)==0) {return ;}
	else if(strncmp(command,"check",5)==0 || strncmp(command,"1",1)==0 || strncmp(command,"c",1)==0)
	{
		while(1){
			kollmorgen_unit_conversion(1);
		   // printf("[1]:%6.2lf [2]:%6.2lf [3]:%6.2lf [4]:%6.2lf [5]:%6.2lf [6]:%6.2lf \n",ActualPosition_ph[1],ActualPosition_ph[2],ActualPosition_ph[3],ActualPosition_ph[4],ActualPosition_ph[5],ActualPosition_ph[6]);
		    p[0]=0;
			p[1]=ActualPosition_ph[1];
			p[2]=ActualPosition_ph[2];
			p[3]=ActualPosition_ph[3];
			p[4]=ActualPosition_ph[4];
			p[5]=ActualPosition_ph[5];
			p[6]=ActualPosition_ph[6];
			gravity_compensation(p,gra_compen); 

			// v_err[1]=ActualVelocity_ph[1]-vl[1];
			// v_err[2]=ActualVelocity_ph[2]-vl[2];
			// v_err[3]=ActualVelocity_ph[3]-vl[3];
			// v_err[4]=ActualVelocity_ph[4]-vl[4];
			// v_err[5]=ActualVelocity_ph[5]-vl[5];
			// v_err[6]=ActualVelocity_ph[6]-vl[6];
		//	kollmorgen_unit_conversion(1);
		// 	if(ActualVelocity_ph[1]>0.1) Ff_com[1]=7.58;  else if(ActualVelocity_ph[1]<-0.1) Ff_com[1]=-7.58; else Ff_com[1]=0;			
		// 	if(ActualVelocity_ph[2]>0.1) Ff_com[2]=9.1;   else if(ActualVelocity_ph[2]<-0.1) Ff_com[2]=-8.34; else Ff_com[2]=0;			
		// 	if(ActualVelocity_ph[3]>0.1) Ff_com[3]=10.62; else if(ActualVelocity_ph[3]<-0.1) Ff_com[3]=-10.62; else Ff_com[3]=0;			
		// 	if(ActualVelocity_ph[4]>0.1) Ff_com[4]=4.07;  else if(ActualVelocity_ph[4]<-0.1) Ff_com[4]=-4.07; else Ff_com[4]=0;			
		// 	if(ActualVelocity_ph[5]>0.1) Ff_com[5]=4.58;  else if(ActualVelocity_ph[5]<-0.1) Ff_com[5]=-4.24; else Ff_com[5]=0;			
		// 	if(ActualVelocity_ph[6]>0.1) Ff_com[6]=4.58;  else if(ActualVelocity_ph[6]<-0.1) Ff_com[6]=-4.75; else Ff_com[6]=0;			
		    

		EnterMutex();
        v[1]=ActualVelocity6;
		v[2]=ActualVelocity5;
		v[3]=ActualVelocity4;
		v[4]=ActualVelocity3;
		v[5]=ActualVelocity2;
		v[6]=ActualVelocity1;
		LeaveMutex();


           Ff_com[1]=0;
		   Ff_com[2]=0;
		   Ff_com[3]=0;
		   Ff_com[4]=0;
		   Ff_com[5]=0;
		   Ff_com[6]=0;
		if(v[1]>1000) Ff_com[1]=8.00;  else if(v[1]<-1000) Ff_com[1]=-8.00; else Ff_com[1]=0;			
		if(v[2]>100) Ff_com[2]=4;   else if(v[2]<-1000) Ff_com[2]=-4; else Ff_com[2]=0;			
        if(v[3]>100) Ff_com[3]=-6; else if(v[3]<-1000) Ff_com[3]=6; else Ff_com[3]=0;			
		if(v[4]>5) Ff_com[4]=3.0;  else if(v[4]<-1000) Ff_com[4]=-3.0; else Ff_com[4]=0;			
		if(v[5]>5) Ff_com[5]=4.5;  else if(v[5]<-1000) Ff_com[5]=-4.5; else Ff_com[5]=0;			
		if(v[6]>5) Ff_com[6]=5;  else if(v[6]<-1000) Ff_com[6]=-5; else Ff_com[6]=0;			
		

		//打印数据
		   	 //printf("[1]:%d [2]:%d [3]:%d [4]:%d [5]:%d [6]:%d \n",v[1],v[2],v[3],v[4],v[5],v[6]);

		
	
			TargetTorque_ph[1]= gra_compen[1]+Ff_com[1];kollmorgen_unit_conversion(0);kollmorgen_move(1);
			TargetTorque_ph[2]= gra_compen[2]+Ff_com[2];kollmorgen_unit_conversion(0);kollmorgen_move(2);
			TargetTorque_ph[3]= gra_compen[3]+Ff_com[3];kollmorgen_unit_conversion(0);kollmorgen_move(3);
			TargetTorque_ph[4]= gra_compen[4]+Ff_com[4];kollmorgen_unit_conversion(0);kollmorgen_move(4);
			TargetTorque_ph[5]= gra_compen[5]+Ff_com[5];kollmorgen_unit_conversion(0);kollmorgen_move(5);
			TargetTorque_ph[6]= gra_compen[6]+Ff_com[6];kollmorgen_unit_conversion(0);kollmorgen_move(6);

		}
		
		 

	}

	
}
void fourier_tra(){
	OperationMode1 = 0x03;
	OperationMode2 = 0x03;
	OperationMode3 = 0x03;
	OperationMode4 = 0x03;
	OperationMode5 = 0x03;
	OperationMode6 = 0x03;
	TargetVelocity_ph[1]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(1);
	TargetVelocity_ph[2]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(2);
	TargetVelocity_ph[3]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(3);
	TargetVelocity_ph[4]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(4);
	TargetVelocity_ph[5]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(5);
	TargetVelocity_ph[6]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(6);

	system("clear");
	char command[200];	
	printf("****************************************\n");
	printf("*             激励轨迹                 *\n");
	printf("*      exit      :退出关节控制模式     *\n");
	printf("*      run5      :执行5     *\n");
	printf("*      run4      :执行4号轨迹:20s-慢速  *\n");
	printf("*      run3      :执行3号轨迹:10s-慢速  *\n");
	printf("*      run2      :执行2号轨迹:10s-快速  *\n");
	printf("*      run1      :执行1号轨迹:10s-快速  *\n");
	printf("*      run0      :    *\n");
    printf("****************************************\n");
    printf("输入控制指令/n:");
	fgets(command, sizeof(command), stdin);
	    if(strncmp(command,"exit",4)==0 || strncmp(command,"e",1)==0) {return ;}
	else if(strncmp(command,"run4",4)==0  || strncmp(command,"r4",2)==0) {fourier_tra_move4();}
    else if(strncmp(command,"run3",4)==0  || strncmp(command,"r3",2)==0) {fourier_tra_move3();}
    else if(strncmp(command,"run2",4)==0  || strncmp(command,"r2",2)==0) {fourier_tra_move2();}
    else if(strncmp(command,"run1",4)==0  || strncmp(command,"r1",2)==0) {fourier_tra_move1();}
	}


		
void fourier_tra_move1(){
	char command[200];
	OperationMode1 = 0x01;
	OperationMode2 = 0x01;
	OperationMode3 = 0x01;
	OperationMode4 = 0x01;
	OperationMode5 = 0x01;
	OperationMode6 = 0x04;
	double t2;
	for (t=0;t<=10;t=t+0.01)
    {
	  t2=fmod(t,10);
	  usleep(10*1000);
	  fourier_tra_pos1(t2,p);  

      kollmorgen_unit_conversion(1);
	  printf("%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f\n",t,ActualPosition_ph[1],ActualPosition_ph[2],ActualPosition_ph[3],ActualPosition_ph[4],ActualPosition_ph[5],ActualPosition_ph[6],\
	                                                                                              ActualTorque_ph[1],ActualTorque_ph[2],ActualTorque_ph[3],ActualTorque_ph[4],ActualTorque_ph[5],ActualTorque_ph[6]);
       
	   
	   mode_move_4_position(6,p[5]);
	   mode_move_4_position(5,p[4]);
	   mode_move_4_position(4,p[3]);
	   mode_move_4_position(3,p[2]);
	   mode_move_4_position(2,p[1]);
	   mode_move_4_position(1,p[0]);
	} 
	printf("是否退出？(yes/no)/n:");
	fgets(command, sizeof(command), stdin);
	    if(strncmp(command,"yes",3)==0 || strncmp(command,"y",1)==0) {return;}
		else
		{
			return;
		}}

void fourier_tra_move2(){
	char command[200];
	OperationMode1 = 0x01;
	OperationMode2 = 0x01;
	OperationMode3 = 0x01;
	OperationMode4 = 0x01;
	OperationMode5 = 0x01;
	OperationMode6 = 0x04;
	double t2;
	for (t=0;t<=10;t=t+0.01)
    {
	  t2=fmod(t,10);
	  usleep(10*1000);
	  fourier_tra_pos2(t2,p); 

      kollmorgen_unit_conversion(1);
	  printf("%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f\n",t,ActualPosition_ph[1],ActualPosition_ph[2],ActualPosition_ph[3],ActualPosition_ph[4],ActualPosition_ph[5],ActualPosition_ph[6],\
	                                                                                              ActualTorque_ph[1],ActualTorque_ph[2],ActualTorque_ph[3],ActualTorque_ph[4],ActualTorque_ph[5],ActualTorque_ph[6]);
      
       mode_move_4_position(6,p[5]);
	   mode_move_4_position(5,p[4]);
	   mode_move_4_position(4,p[3]);
	   mode_move_4_position(3,p[2]);
	   mode_move_4_position(2,p[1]);
	   mode_move_4_position(1,p[0]);
	} 
	printf("是否退出？(yes/no)/n:");
	fgets(command, sizeof(command), stdin);
	    if(strncmp(command,"yes",3)==0 || strncmp(command,"y",1)==0) {return;}
		else
		{
			return;
		}}


void fourier_tra_move3(){
	char command[200];
	OperationMode1 = 0x01;
	OperationMode2 = 0x01;
	OperationMode3 = 0x01;
	OperationMode4 = 0x01;
	OperationMode5 = 0x01;
	OperationMode6 = 0x04;
	double t2;
	for (t=0;t<=10;t=t+0.01)
    {
	  t2=fmod(t,10);
	  usleep(10*1000);
	  fourier_tra_pos3(t2,p);  

	  kollmorgen_unit_conversion(1);
	  printf("%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f\n",t,ActualPosition_ph[1],ActualPosition_ph[2],ActualPosition_ph[3],ActualPosition_ph[4],ActualPosition_ph[5],ActualPosition_ph[6],\
	                                                                                              ActualTorque_ph[1],ActualTorque_ph[2],ActualTorque_ph[3],ActualTorque_ph[4],ActualTorque_ph[5],ActualTorque_ph[6]);
      
      
	  

       mode_move_4_position(6,p[5]);
	   mode_move_4_position(5,p[4]);
	   mode_move_4_position(4,p[3]);
	   mode_move_4_position(3,p[2]);
	   mode_move_4_position(2,p[1]);
	   mode_move_4_position(1,p[0]);
	} 
	printf("是否退出？(yes/no)/n:");
	fgets(command, sizeof(command), stdin);
	    if(strncmp(command,"yes",3)==0 || strncmp(command,"y",1)==0) {return;}
		else
		{
			return;
		}}

void fourier_tra_move4(){
	char command[200];
	OperationMode1 = 0x01;
	OperationMode2 = 0x01;
	OperationMode3 = 0x01;
	OperationMode4 = 0x01;
	OperationMode5 = 0x01;
	OperationMode6 = 0x04;
	double t2;
	for (t=0;t<=20;t=t+0.01)
    {
	  t2=fmod(t,20);
	  usleep(10*1000);
	  fourier_tra_pos4(t2,p);  
	   kollmorgen_unit_conversion(1);
	  printf("%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f\n",t,ActualPosition_ph[1],ActualPosition_ph[2],ActualPosition_ph[3],ActualPosition_ph[4],ActualPosition_ph[5],ActualPosition_ph[6],\
	                                                                                              ActualTorque_ph[1],ActualTorque_ph[2],ActualTorque_ph[3],ActualTorque_ph[4],ActualTorque_ph[5],ActualTorque_ph[6]);
      
       mode_move_4_position(6,p[5]);
	   mode_move_4_position(5,p[4]);
	   mode_move_4_position(4,p[3]);
	   mode_move_4_position(3,p[2]);
	   mode_move_4_position(2,p[1]);
	   mode_move_4_position(1,p[0]);
	} 
	printf("是否退出？(yes/no)/n:");
	fgets(command, sizeof(command), stdin);
	    if(strncmp(command,"yes",3)==0 || strncmp(command,"y",1)==0) {return;}
		else
		{
			return;
		}}

int kollmorgen_help(){
	system("clear");
	char command[200];
	printf("****************************************\n");
	printf("*             帮助说明                 *\n");
	printf("*    help:帮助命令-显示功能按键        *\n");
	printf("*    test:测试模式                     *\n");
	printf("*      qi  :关节i正向运动              *\n");
    printf("*      qqi :关节i反向运动              *\n");
	printf("*      zz  :全部关节停止运动           *\n");
	printf("*      exit:退出测试模式               *\n");
	printf("*    mode:关节模式控制                 *\n");
	printf("*      ji        :选择控制关节         *\n");
	printf("*      1-speed   :关节i的改为速度模式  *\n");
	printf("*      2-position:关节i的改为位置模式  *\n");
	printf("*      3-torque  :关节i的改为电流模式  *\n");
	printf("*      输入执行数值:数值               *\n");
	printf("*      run       :执行电机运动         *\n");
	printf("*      stop      :终止电机运动         *\n");
    printf("*      exit      :退出关节运动         *\n");
	printf("*      exit      :退出关节控制模式     *\n");
	printf("****************************************\n");
    printf("是否退出帮助模式y/n:");
	fgets(command, sizeof(command), stdin);
	while(strncmp(command,"n",1)==0)
		{
			fgets(command, sizeof(command), stdin);
		}
	return 0;
	}
void CheckReadSDO(CO_Data* d, UNS8 nodeid){
	UNS32 abortCode;
	UNS32 data=0;
	UNS32 size=64;
	int ret = 0;

	ret = getReadResultNetworkDict(&TestMaster_Data, nodeid, &data, &size, &abortCode);
	if(ret != SDO_FINISHED)
		printf("\nResult : Failed in getting information for slave %2.2x, AbortCode :%4.4x \n", nodeid, abortCode);
	else
		printf("\nResult : %x\n", data);

	/* Finalize last SDO transfer with this node */
	closeSDOtransfer(&TestMaster_Data, nodeid, SDO_CLIENT);}
int kollmorgen_check(){
    int num;
	int i=0;
	UNS8 nodeid;
	UNS16 index;
	UNS16 subindex;
	char command[200];
	system("clear");
	while (1)
	{
		if(i==0)
		{
			printf("*******************************\n");
			printf("*      实时监控(单次)         *\n");
		    printf("* 电机选择[all(0) 1 2 3 4 5 6]*\n");
			printf("*   多次模式 [check] | [c]    *\n");
			printf("*   查看力传感器数据 [fsensor] | [f]    *\n");
			printf("*        退出[exit]/[e]       *\n");
			printf("*******************************\n");
			i++;
		}
		printf("请选择电机:");
		fgets(command, sizeof(command), stdin);
			 if(strncmp(command,"exit",4)==0 || strncmp(command,"e",1)==0) {return 0;}
	    else if(strncmp(command,"all",3)==0  || strncmp(command,"0",1)==0) {kollmorgen_display_ph(0,1);}
        else if(strncmp(command,"j1",2)==0   || strncmp(command,"1",1)==0) {kollmorgen_display(1,1);}
		else if(strncmp(command,"j2",2)==0   || strncmp(command,"2",1)==0) {kollmorgen_display(2,1);}
		else if(strncmp(command,"j3",2)==0   || strncmp(command,"3",1)==0) {kollmorgen_display(3,1);}
		else if(strncmp(command,"j4",2)==0   || strncmp(command,"4",1)==0) {kollmorgen_display(4,1);}
		else if(strncmp(command,"j5",2)==0   || strncmp(command,"5",1)==0) {kollmorgen_display(5,1);}
		else if(strncmp(command,"j6",2)==0   || strncmp(command,"6",1)==0) {kollmorgen_display(6,1);}
		else if(strncmp(command,"fsensor",2)==0   || strncmp(command,"f",1)==0) {Fdata_get();}
		else if(strncmp(command,"check",5)==0 || strncmp(command,"c",1)==0)
		{
			int j=0;
			system("clear");
			while (1)
			{
				if (j==0)
				{
					printf("*******************************\n");
					printf("*      实时监控(多次)         *\n");
					printf("* 电机选择[all(0) 1 2 3 4 5 6]*\n");
					printf("*        退出[exit]/[e]       *\n");
					printf("*******************************\n");
					j++;
				}
				printf("请输入电机/次数(空格):");
				scanf("%s%d",command,&num);
				
					     if(strncmp(command,"exit",4)==0 || strncmp(command,"e",1)==0){break;}
				    else if(strncmp(command,"all",3)==0  || strncmp(command,"0",1)==0) {kollmorgen_display_ph(0,num);}
			    	else if(strncmp(command,"j1",2)==0   || strncmp(command,"1",1)==0) {kollmorgen_display(1,num);}
					else if(strncmp(command,"j2",2)==0   || strncmp(command,"2",1)==0) {kollmorgen_display(2,num);}
					else if(strncmp(command,"j3",2)==0   || strncmp(command,"3",1)==0) {kollmorgen_display(3,num);}
					else if(strncmp(command,"j4",2)==0   || strncmp(command,"4",1)==0) {kollmorgen_display(4,num);}
					else if(strncmp(command,"j5",2)==0   || strncmp(command,"5",1)==0) {kollmorgen_display(5,num);}
					else if(strncmp(command,"j6",2)==0   || strncmp(command,"6",1)==0) {kollmorgen_display(6,num);}

					
				
			}
			i=0;
			system("clear");
		}
		else if(strncmp(command,"q1",2)==0 )
		{
			// nodeid = 0x07;  //ks :24  kp:0c
			// index = 0x20D8;
			// subindex = 0x24;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			// nodeid = 0x07;   //ki
			// index = 0x2032;
			// subindex = 0x08;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			nodeid = 0x07; //kr 03分子 02分母
			index = 0x2032;
			subindex = 0x0E;
			readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		}
		else if(strncmp(command,"qq1",3)==0 )
		{
			// nodeid = 0x07;  //ks :24  kp:0c
			// index = 0x20D8;
			// subindex = 0x24;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			// nodeid = 0x07;   //ki
			// index = 0x2032;
			// subindex = 0x08;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			nodeid = 0x07; //kr 03分子 02分母
			index = 0x2032;
			subindex = 0x0D;
			readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		}
		else if(strncmp(command,"qt1",3)==0 )
		{
			// nodeid = 0x07;  //ks :24  kp:0c
			// index = 0x20D8;
			// subindex = 0x24;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			// nodeid = 0x07;   //ki
			// index = 0x2032;
			// subindex = 0x08;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			nodeid = 0x07; //kr 03分子 02分母
			index = 0x2027;
			subindex = 0x03;
			readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		}
		else if(strncmp(command,"q2",2)==0 )
		{
			// nodeid = 0x06;  //ks :24  kp:0c
			// index = 0x20D8;
			// subindex = 0x24; 
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			// nodeid = 0x06;   //ki
			// index = 0x2032;
			// subindex = 0x08;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
            nodeid = 0x06; //kr 03分子 02分母
			index = 0x201E;
			subindex = 0x02;
			readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		}
		else if(strncmp(command,"q3",2)==0 )
		{
			// nodeid = 0x05;  //ks :24  kp:0c
			// index = 0x20D8;
			// subindex = 0x24;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			// nodeid = 0x05;   //ki
			// index = 0x2032;
			// subindex = 0x08;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			nodeid = 0x05; //kr 03分子 02分母
			index = 0x201C;
			subindex = 0x03;
			readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		}
		else if(strncmp(command,"q4",2)==0 )
		{
			// nodeid = 0x04;  //ks :24  kp:0c
			// index = 0x20D8;
			// subindex = 0x24;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			// nodeid = 0x04;   //ki
			// index = 0x2032;
			// subindex = 0x08;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			nodeid = 0x04; //kr 03分子 02分母
			index = 0x201C;
			subindex = 0x03;
			readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		}
		else if(strncmp(command,"q5",2)==0 )
		{
			// nodeid = 0x03;  //ks :24  kp:0c
			// index = 0x20D8;
			// subindex = 0x24;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			// nodeid = 0x03;   //ki
			// index = 0x2032;
			// subindex = 0x08;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			nodeid = 0x03; //kr 03分子 02分母
			index = 0x201C;
			subindex = 0x03;
			readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		}
		else if(strncmp(command,"q6",2)==0 )
		{
			// nodeid = 0x02;  //ks :24  kp:0c
			// index = 0x20D8;
			// subindex = 0x24;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
			// nodeid = 0x02;  //ki
			// index = 0x2032;
			// subindex = 0x08;
			// readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		    nodeid = 0x02; //kr 03分子 02分母
			index = 0x201C;
			subindex = 0x03;
			readNetworkDictCallback (&TestMaster_Data, nodeid, index, subindex, 0, CheckReadSDO, 0);
		}
		else if(strncmp(command,"k",1)==0 )
		{
			printf("当前模式:%d/n",Operation_Mode);
		}
		
	}
    return 0;	}
int kollmorgen_display2(int joint,int num){
	int i;
	if(joint==0)
	{
        
		printf("\n");
		for(i=1;i<=num;i++)
	    printf("[1]P:%d V:%d T:%d [2]P:%d V:%d T:%d [3]P:%d V:%d T:%d [4]P:%d V:%d T:%d [5]P:%d V:%d T:%d [6]P:%d V:%d T:%d\n",\
		                                        ActualPosition6,ActualVelocity6,ActualTorque6,\
												ActualPosition5,ActualVelocity5,ActualTorque5,\
												ActualPosition4,ActualVelocity4,ActualTorque4,\
												ActualPosition3,ActualVelocity3,ActualTorque3,\
												ActualPosition2,ActualVelocity2,ActualTorque2,\
												ActualPosition1,ActualVelocity1,ActualTorque1);printf("\n");
	}
	else
	{
		switch (joint) {
		case 1: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition6,ActualVelocity6,ActualTorque6);printf("\n");break;}
		case 2: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition5,ActualVelocity5,ActualTorque5);printf("\n");break;}
		case 3: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition4,ActualVelocity4,ActualTorque4);printf("\n");break;}
		case 4: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition3,ActualVelocity3,ActualTorque3);printf("\n");break;}
		case 5: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition2,ActualVelocity2,ActualTorque2);printf("\n");break;}
		case 6: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition1,ActualVelocity1,ActualTorque1);printf("\n");break;}
		}
	}
	}
int kollmorgen_display(int joint,int num){
	int i;
	if(joint==0)
	{
   
		printf("\n");
		for(i=1;i<=num;i++)
	    printf("[1]P:%d V:%d T:%d [2]P:%d V:%d T:%d [3]P:%d V:%d T:%d [4]P:%d V:%d T:%d [5]P:%d V:%d T:%d [6]P:%d V:%d T:%d\n",\
		                                        ActualPosition6,ActualVelocity6,ActualTorque6,\
												ActualPosition5,ActualVelocity5,ActualTorque5,\
												ActualPosition4,ActualVelocity4,ActualTorque4,\
												ActualPosition3,ActualVelocity3,ActualTorque3,\
												ActualPosition2,ActualVelocity2,ActualTorque2,\
												ActualPosition1,ActualVelocity1,ActualTorque1);printf("\n");
	}
	else
	{
		switch (joint) {
		case 1: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition6,ActualVelocity6,ActualTorque6);printf("\n");break;}
		case 2: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition5,ActualVelocity5,ActualTorque5);printf("\n");break;}
		case 3: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition4,ActualVelocity4,ActualTorque4);printf("\n");break;}
		case 4: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition3,ActualVelocity3,ActualTorque3);printf("\n");break;}
		case 5: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition2,ActualVelocity2,ActualTorque2);printf("\n");break;}
		case 6: { printf("\n");for(i=1;i<=num;i++) printf("[%d]P:%d V:%d T:%d\n",joint,ActualPosition1,ActualVelocity1,ActualTorque1);printf("\n");break;}
		}
	}}
int kollmorgen_display_ph(int joint,int num){
	int i;
	if(joint==0)
	{
        kollmorgen_unit_conversion(1);
		printf("\n");
		for(i=1;i<=num;i++)
        for(int k=1;k<=6;k++)
		    printf("[%d]P:%6.2lf V:%5.2lf T:%6.2lf \n",k,ActualPosition_ph[k],ActualVelocity_ph[k],ActualTorque_ph[k]);
	    printf("\n");
		//printf("[1]P:%d V:%d T:%d\n",ActualPosition6,ActualVelocity6,ActualTorque6);
	}
	else
	{
		kollmorgen_unit_conversion(1);
		printf("\n");
		for(i=1;i<=num;i++)
		    printf("[%d]P:%lf V:%lf T:%lf\n",joint,ActualPosition_ph[joint],ActualVelocity_ph[joint],ActualTorque_ph[joint]);
		printf("\n");
	}}
/**
 * 注意有三种工作模式
 * 0-将"目标物理值"转化为"目标驱动值":位置\速度\电流 p-m
 * 1-将"实际驱动值"转化为"实际物理值":位置\速度\电流 m-p
*/
int kollmorgen_unit_conversion(int direction)
{
	int ap[7]={0,0,0,262144,0,262144,0};//位置-初始偏差(输入单位)
	int ep[7]={0,37033,8723,-4371,13109,2,-16019}; //位置-调整偏差(输入单位)

	if (direction==0)
	{
		for(int k=1;k<=6;k++)
			TargetPosition[k]=TargetPosition_ph[k]*65536/45+ap[k]-ep[k];
		
		TargetPosition[3]=-TargetPosition_ph[3]*65536/45+ap[3]-ep[3];
		
		for(int k=1;k<=6;k++)
			TargetVelocity[k]=TargetVelocity_ph[k]*1847221816/9375;

		TargetVelocity[3]=-TargetVelocity[3];
        
		TargetTorque[1]=4*TargetTorque_ph[1]*819200.0/24846;
		TargetTorque[2]=4*TargetTorque_ph[2]*819200.0/24846;
		TargetTorque[3]=-4*TargetTorque_ph[3]*819200.0/24846;
		TargetTorque[4]=4*TargetTorque_ph[4]*81920.0/1111;
		TargetTorque[5]=4*TargetTorque_ph[5]*81920.0/1111;
		TargetTorque[6]=4*TargetTorque_ph[6]*81920.0/1111;
	}
	else if (direction==1)
	{
		ActualPosition_ph[1]=(ActualPosition6+ep[1]-ap[1])*45.0/65536.0;
		ActualPosition_ph[2]=(ActualPosition5+ep[2]-ap[2])*45.0/65536.0;
		ActualPosition_ph[3]=-(ActualPosition4+ep[3]-ap[3])*45.0/65536.0;
		ActualPosition_ph[4]=(ActualPosition3+ep[4]-ap[4])*45.0/65536.0;
		ActualPosition_ph[5]=(ActualPosition2+ep[5]-ap[5])*45.0/65536.0;
		ActualPosition_ph[6]=(ActualPosition1+ep[6]-ap[6])*45.0/65536.0;

		ActualVelocity_ph[1]=ActualVelocity6*9375/1847221816.0;
		ActualVelocity_ph[2]=ActualVelocity5*9375/1847221816.0;
		ActualVelocity_ph[3]=-ActualVelocity4*9375/1847221816.0;
		ActualVelocity_ph[4]=ActualVelocity3*9375/1847221816.0;
		ActualVelocity_ph[5]=ActualVelocity2*9375/1847221816.0;
		ActualVelocity_ph[6]=ActualVelocity1*9375/1847221816.0;

		ActualTorque_ph[1]=ActualTorque6*24846/819200.0;
		ActualTorque_ph[2]=ActualTorque5*24846/819200.0;
		ActualTorque_ph[3]=-ActualTorque4*24846/819200.0;
		ActualTorque_ph[4]=ActualTorque3*1111/81920.0;
		ActualTorque_ph[5]=ActualTorque2*1111/81920.0;
		ActualTorque_ph[6]=ActualTorque1*1111/81920.0;
	}
	else if (direction==2)
	{
		for(int k=1;k<=6;k++)
		{
			group0_ph[k]=(group0[k]+ep[k]-ap[k])*45.0/65536.0;	
			group1_ph[k]=(group1[k]+ep[k]-ap[k])*45.0/65536.0;	
			group2_ph[k]=(group2[k]+ep[k]-ap[k])*45.0/65536.0;	
		}
	}

	if (direction==3)//关节1的速度转化专用如果用==0的时候会出现targetposition也变化的情况
	{
		for(int k=1;k<=6;k++)
		TargetVelocity[k]=TargetVelocity_ph[k]*1847221816/9375;
	}
}
int kollmorgen_mode_move() {//电机选择
	
	
	char command[200];
	while (1)
	{
		system("clear");
		printf("*******************************\n");
		printf("*         模式控制            *\n");
		printf("*电机选择[j1 j2 j3 j4 j5 j6 ] *\n");
		printf("*电机选择[ 1  2  3  4  5  6 ] *\n");
		printf("*        监控[check/c]        *\n");
		printf("*        退出[exit]/[e]       *\n");
		printf("*******************************\n");
		printf("请选择电机:");
		fgets(command, sizeof(command), stdin);
		if(strncmp(command,"exit",4)==0 || strncmp(command,"e",1)==0)
		{
			printf("结束模式控制\n");
			return 0;
		}
		
		else if(strncmp(command,"check",5)==0|| strncmp(command,"c",1)==0){kollmorgen_check();}
	    else if(strncmp(command,"j1",2)==0) {mode_move_1(1);}
		else if(strncmp(command,"j2",2)==0) {mode_move_1(2);}
		else if(strncmp(command,"j3",2)==0) {mode_move_1(3);}
		else if(strncmp(command,"j4",2)==0) {mode_move_1(4);}
		else if(strncmp(command,"j5",2)==0) {mode_move_1(5);}
		else if(strncmp(command,"j6",2)==0) {mode_move_1(6);}
		else if(strncmp(command,"1",1)==0) {mode_move_1(1);}
		else if(strncmp(command,"2",1)==0) {mode_move_1(2);}
		else if(strncmp(command,"3",1)==0) {mode_move_1(3);}
		else if(strncmp(command,"4",1)==0) {mode_move_1(4);}
		else if(strncmp(command,"5",1)==0) {mode_move_1(5);}
		else if(strncmp(command,"6",1)==0) {mode_move_1(6);}
		else 
		{
			printf("指令错误\n");
		}
	}
    return 0;	}
int mode_move_1(int joint){ //模式选择
	
	char command[200];
	while(1)
	{
		system("clear");
		printf("****************************\n");
		printf("*     %d号电机模式控制      *\n",joint);
		printf("*    模式选择[1-m1-位置]   *\n");
		printf("*    模式选择[2-m2-速度]   *\n");
		printf("*    模式选择[3-m3-力矩]   *\n");
		printf("*      监控[check/c]       *\n");
		printf("*      退出[exit /e]       *\n");
		printf("****************************\n");
		printf("请选择模式:");
		fgets(command, sizeof(command), stdin);
		if(strncmp(command,"exit",4)==0 || strncmp(command,"e",1)==0)
		{
			printf("结束模式控制\n");
			return 0;
		}
		else if(strncmp(command,"check",5)==0|| strncmp(command,"c",1)==0){kollmorgen_check();}
	    else if(strncmp(command,"m1",2)==0) {mode_move_2(joint,1);}
		else if(strncmp(command,"m2",2)==0) {mode_move_2(joint,2);}
		else if(strncmp(command,"m3",2)==0) {mode_move_2(joint,3);}
		else if(strncmp(command,"1",1)==0) {mode_move_2(joint,1);}
		else if(strncmp(command,"2",1)==0) {mode_move_2(joint,2);}
		else if(strncmp(command,"3",1)==0) {mode_move_2(joint,3);}
		else 
		{
			printf("指令错误\n");
		}
	}	
	return 0;}
int mode_move_2(int joint,int mode){//选择数值
	
	char command[200];
	double num;
	if (mode==1)
	{
		EnterMutex();
		TargetPosition1=ActualPosition1;
		TargetPosition2=ActualPosition2;
		TargetPosition3=ActualPosition3;
		TargetPosition4=ActualPosition4;
		TargetPosition5=ActualPosition5;
		TargetPosition6=ActualPosition6;
		LeaveMutex();
	}
		if (mode==3)
	{
		EnterMutex();
		// TargetTorque1=ActualTorque1;
		// TargetTorque2=ActualTorque2;
		// TargetTorque3=ActualTorque3;
		// TargetTorque4=ActualTorque4;
		// TargetTorque5=ActualTorque5;
		// TargetTorque6=ActualTorque6;
		LeaveMutex();
	}
	switch (joint)
		{
			EnterMutex();
			case 1: switch(mode) {case 1: OperationMode6=0x04;break;case 2: OperationMode6=0x03;break;case 3: OperationMode6=0x04;break;}break;
			case 2: switch(mode) {case 1: OperationMode5=0x01;break;case 2: OperationMode5=0x03;break;case 3: OperationMode5=0x04;break;}break;
			case 3: switch(mode) {case 1: OperationMode4=0x01;break;case 2: OperationMode4=0x03;break;case 3: OperationMode4=0x04;break;}break;
			case 4: switch(mode) {case 1: OperationMode3=0x01;break;case 2: OperationMode3=0x03;break;case 3: OperationMode3=0x04;break;}break;
			case 5: switch(mode) {case 1: OperationMode2=0x01;break;case 2: OperationMode2=0x03;break;case 3: OperationMode2=0x04;break;}break;
			case 6: switch(mode) {case 1: OperationMode1=0x01;break;case 2: OperationMode1=0x03;break;case 3: OperationMode1=0x04;break;}break;
		    LeaveMutex();
		}
		
	while(1)
	{
		// system("clear");
		printf("**********************************\n");
		printf("*      %d号电机模式控制           *\n",joint);
		switch (mode)
		{
			case 1:{printf("*      模式控制:位置(%d)      *\n",ActualPosition6);break;}
			case 2:{printf("*      模式控制:速度             *\n");break;}
			case 3:{printf("*      模式控制:力矩             *\n");break;}
		}
		printf("*      mode[   %d   ]           *\n",OperationMode2);
		printf("*      输入数值[     ]           *\n");
		printf("*运行[run] 退出[exit] 查看[check]*\n");
		printf("**********************************\n");
		printf("e&r&c:");
		fgets(command, sizeof(command), stdin);
		if(strncmp(command,"exit",4)==0 || strncmp(command,"e",1)==0)
		{
			printf("结束模式控制\n");
			return 0;
		}
		else if(strncmp(command,"check",5)==0|| strncmp(command,"c",1)==0){kollmorgen_check();}
		else if(strncmp(command,"run",3)==0  ||strncmp(command,"r",1)==0)
		{
			while(1)
			{
				if(pos_control_invel==0)
				{
					// printf("现在的pos指标:%d\n",pos_control_invel);
					printf("请输入数值(只可以输入数值|0退出):");
					scanf("%lf",&num);
					
					if (fabs(num-0)<0.0001) 
					{
						move_stop(0);
						break;
					}
					else
					{
						switch (mode)
						{
							case 1:{mode_move_4_position(joint,num);break;}
							//printf("%d号-位置模式:%d-[理论:%d-实际:%d]\n",joint,OperationMode6,TargetPosition6,ActualPosition6);
							case 2:{mode_move_4_velocity(joint,num);printf("%d号-速度模式-[%lf]\n",joint,num);break;}                                                            
							case 3:{mode_move_4_current(joint,num);break;}
						}
					}
				}
			}
		}
		else 
		{
			printf("错误!\n");
		}
	
	}	
	return 0;}

int move_stop(int joint)
{
	if (joint==0)
	{
		switch(joint)
		{
		EnterMutex();
		case 0: switch(OperationMode6) {case 1: TargetPosition6= ActualPosition6;kollmorgen_unit_conversion(0);kollmorgen_move(1);break;case 3: TargetVelocity_ph[1]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(1);break;case 4: ;break;};
		case 1: switch(OperationMode5) {case 1: TargetPosition5= ActualPosition5;kollmorgen_unit_conversion(0);kollmorgen_move(2);break;case 3: TargetVelocity_ph[2]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(2);break;case 4: ;break;};
		case 2: switch(OperationMode4) {case 1: TargetPosition4= ActualPosition4;kollmorgen_unit_conversion(0);kollmorgen_move(3);break;case 3: TargetVelocity_ph[3]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(3);break;case 4: ;break;};
		case 3: switch(OperationMode3) {case 1: TargetPosition3= ActualPosition3;kollmorgen_unit_conversion(0);kollmorgen_move(4);break;case 3: TargetVelocity_ph[4]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(4);break;case 4: ;break;};
		case 4: switch(OperationMode2) {case 1: TargetPosition2= ActualPosition2;kollmorgen_unit_conversion(0);kollmorgen_move(5);break;case 3: TargetVelocity_ph[5]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(5);break;case 4: ;break;};
		case 5: switch(OperationMode1) {case 1: TargetPosition1= ActualPosition1;kollmorgen_unit_conversion(0);kollmorgen_move(6);break;case 3: TargetVelocity_ph[6]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(6);break;case 4: ;break;};
		LeaveMutex();
		}
	}
	else
	{
		switch(joint)
		{
		case 1: switch(OperationMode6) {case 1: TargetPosition_ph[1]= ActualPosition6;kollmorgen_unit_conversion(0);kollmorgen_move(1);break;case 3: TargetVelocity_ph[1]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(1);break;case 4: ;break;}break;
		case 2: switch(OperationMode5) {case 1: TargetPosition_ph[2]= ActualPosition5;kollmorgen_unit_conversion(0);kollmorgen_move(2);break;case 3: TargetVelocity_ph[2]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(2);break;case 4: ;break;}break;
		case 3: switch(OperationMode4) {case 1: TargetPosition_ph[3]= ActualPosition4;kollmorgen_unit_conversion(0);kollmorgen_move(3);break;case 3: TargetVelocity_ph[3]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(3);break;case 4: ;break;}break;
		case 4: switch(OperationMode3) {case 1: TargetPosition_ph[4]= ActualPosition3;kollmorgen_unit_conversion(0);kollmorgen_move(4);break;case 3: TargetVelocity_ph[4]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(4);break;case 4: ;break;}break;
		case 5: switch(OperationMode2) {case 1: TargetPosition_ph[5]= ActualPosition2;kollmorgen_unit_conversion(0);kollmorgen_move(5);break;case 3: TargetVelocity_ph[5]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(5);break;case 4: ;break;}break;
		case 6: switch(OperationMode1) {case 1: TargetPosition_ph[6]= ActualPosition1;kollmorgen_unit_conversion(0);kollmorgen_move(6);break;case 3: TargetVelocity_ph[6]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(6);break;case 4: ;break;}break;
		}
	}
	return(0);
}
int mode_move_4_position(int joint, double num)
{
	switch(joint)
	{
		
		case 1:TargetPosition_ph[1]= num;kollmorgen_unit_conversion(0);pos_control_invel=1;break;
	//	case 1:TargetPosition_ph[1]= num;kollmorgen_unit_conversion(0);kollmorgen_move(1);break;
		case 2:TargetPosition_ph[2]= num;kollmorgen_unit_conversion(0);kollmorgen_move(2);break;
		case 3:TargetPosition_ph[3]= num;kollmorgen_unit_conversion(0);kollmorgen_move(3);break;
		case 4:TargetPosition_ph[4]= num;kollmorgen_unit_conversion(0);kollmorgen_move(4);break;
		case 5:TargetPosition_ph[5]= num;kollmorgen_unit_conversion(0);kollmorgen_move(5);break;
		case 6:TargetPosition_ph[6]= num;kollmorgen_unit_conversion(0);kollmorgen_move(6);;break;
		
	}
	return 0 ;
}
int mode_move_4_velocity(int joint,double num)
{
	//此为物理值控制
	switch(joint)
	{
		case 1:TargetVelocity_ph[1]= num;kollmorgen_unit_conversion(0);kollmorgen_move(1);break;
		case 2:TargetVelocity_ph[2]= num;kollmorgen_unit_conversion(0);kollmorgen_move(2);break;
		case 3:TargetVelocity_ph[3]= num;kollmorgen_unit_conversion(0);kollmorgen_move(3);break;
		case 4:TargetVelocity_ph[4]= num;kollmorgen_unit_conversion(0);kollmorgen_move(4);break;
		case 5:TargetVelocity_ph[5]= num;kollmorgen_unit_conversion(0);kollmorgen_move(5);break;
		case 6:TargetVelocity_ph[6]= num;kollmorgen_unit_conversion(0);kollmorgen_move(6);break;
	}
	
    // 此为驱动值控制
	// switch(joint)
	// {
	// 	case 1:TargetVelocity6=num;printf("%d号-实际速度:%d-目标速度:%d\n",joint,ActualVelocity6,TargetVelocity6);break;
	// 	case 2:TargetVelocity5=num;printf("%d号-实际速度:%d-目标速度:%d\n",joint,ActualVelocity5,TargetVelocity5);break;
	// 	case 3:TargetVelocity4=num;printf("%d号-实际速度:%d-目标速度:%d\n",joint,ActualVelocity4,TargetVelocity4);break;
	// 	case 4:TargetVelocity3=num;printf("%d号-实际速度:%d-目标速度:%d\n",joint,ActualVelocity3,TargetVelocity3);break;
	// 	case 5:TargetVelocity2=num;printf("%d号-实际速度:%d-目标速度:%d\n",joint,ActualVelocity2,TargetVelocity2);break;
	// 	case 6:TargetVelocity1=num;printf("%d号-实际速度:%d-目标速度:%d\n",joint,ActualVelocity1,TargetVelocity1);break;


	// }
	return 0 ;
}


int mode_move_4_current(int joint,double num)
{
	switch(joint)
	{
		// case 1:TargetTorque_ph[1]= num;kollmorgen_unit_conversion(0);kollmorgen_move(1);printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque6,TargetTorque6);break;
		// case 2:TargetTorque_ph[2]= num;kollmorgen_unit_conversion(0);kollmorgen_move(2);printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque5,TargetTorque5);break;
		// case 3:TargetTorque_ph[3]= num;kollmorgen_unit_conversion(0);kollmorgen_move(3);printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque4,TargetTorque4);break;
		// case 4:TargetTorque_ph[4]= num;kollmorgen_unit_conversion(0);kollmorgen_move(4);printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque3,TargetTorque3);break;
		// case 5:TargetTorque_ph[5]= num;kollmorgen_unit_conversion(0);kollmorgen_move(5);printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque2,TargetTorque2);break;
		// case 6:TargetTorque_ph[6]= num;kollmorgen_unit_conversion(0);kollmorgen_move(6);printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque1,TargetTorque1);break;

        case 1:TargetTorque_ph[1]= num;kollmorgen_unit_conversion(0);kollmorgen_move(1);break;
        case 2:TargetTorque_ph[2]= num;kollmorgen_unit_conversion(0);kollmorgen_move(2);break;
        case 3:TargetTorque_ph[3]= num;kollmorgen_unit_conversion(0);kollmorgen_move(3);break;
        case 4:TargetTorque_ph[4]= num;kollmorgen_unit_conversion(0);kollmorgen_move(4);break;
        case 5:TargetTorque_ph[5]= num;kollmorgen_unit_conversion(0);kollmorgen_move(5);break;
        case 6:TargetTorque_ph[6]= num;kollmorgen_unit_conversion(0);kollmorgen_move(6);break;


	}

	//此为驱动值控制
	// 	switch(joint)
	// {
	// 	case 1:TargetTorque6=num;printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque6,TargetTorque6);break;
	// 	case 2:TargetTorque5=num;printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque5,TargetTorque5);break;
	// 	case 3:TargetTorque4=num;printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque4,TargetTorque4);break;
	// 	case 4:TargetTorque3=num;printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque3,TargetTorque3);break;
	// 	case 5:TargetTorque2=num;printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque2,TargetTorque2);break;
	// 	case 6:TargetTorque1=num;printf("%d号-实际电流:%d-目标电流:%d\n",joint,ActualTorque1,TargetTorque1);break;

	// }
	return(0);
}
int kollmorgen_test_move(){
	char command[200];
    
	//while((ch=getch())!=0x1B)
	//默认速度模式  01位置模式 03速度模式 04电流模式
	OperationMode1 = 0x03;
	OperationMode2 = 0x03;
	OperationMode3 = 0x03;
	OperationMode4 = 0x03;
	OperationMode5 = 0x03;
	OperationMode6 = 0x03;
	while(1)
	{
	    system("clear");
		printf("*******************************\n");
		printf("*         测试模式            *\n");
		printf("*正转[q1  q2  q3  q4  q5  q6 ]*\n");
		printf("*反转[qq1 qq2 qq3 qq4 qq5 qq6]*\n");
		printf("*停转[z1  z2  z3  z4  z5  z6 ]*\n");
		printf("*        监控[check/c]        *\n");
		printf("*    全停[zz]   退出[exit/e]  *\n");
		printf("*******************************\n");
		printf("请输入指令:");
	    fgets(command, sizeof(command), stdin);
	    if(strncmp(command,"exit",4)==0 ||strncmp(command,"e",1)==0)
		{
			printf("结束测试模式\n");
			return 0;
		}
		else if(strncmp(command,"check",5)==0|| strncmp(command,"c",1)==0){kollmorgen_check();}
	 	     if(strncmp(command,"q1",2)==0) {TargetVelocity_ph[1]= 1;kollmorgen_unit_conversion(0);kollmorgen_move(1);printf("执行命令:关节1正向旋转-%s\n",command);		printf("\n状态1:%d,2:%d,3:%d,4:%d,5:%d,6:%d:",OperationMode1,OperationMode2,OperationMode3,OperationMode4,OperationMode5,OperationMode6);}
		else if(strncmp(command,"q2",2)==0) {TargetVelocity_ph[2]= 1;kollmorgen_unit_conversion(0);kollmorgen_move(2);printf("执行命令:关节2正向旋转-%s\n",command);}
		else if(strncmp(command,"q3",2)==0) {TargetVelocity_ph[3]= 1;kollmorgen_unit_conversion(0);kollmorgen_move(3);printf("执行命令:关节3正向旋转-%s\n",command);}
		else if(strncmp(command,"q4",2)==0) {TargetVelocity_ph[4]= 1;kollmorgen_unit_conversion(0);kollmorgen_move(4);printf("执行命令:关节4正向旋转-%s\n",command);}
		else if(strncmp(command,"q5",2)==0) {TargetVelocity_ph[5]= 1;kollmorgen_unit_conversion(0);kollmorgen_move(5);printf("执行命令:关节5正向旋转-%s\n",command);}
		else if(strncmp(command,"q6",2)==0) {TargetVelocity_ph[6]= 1;kollmorgen_unit_conversion(0);kollmorgen_move(6);printf("执行命令:关节6正向旋转-%s\n",command);}
		
		else if(strncmp(command,"qq1",3)==0) {TargetVelocity_ph[1]= -1;kollmorgen_unit_conversion(0);kollmorgen_move(1);printf("执行命令:关节1反向旋转-%s\n",command);}
		else if(strncmp(command,"qq2",3)==0) {TargetVelocity_ph[2]= -1;kollmorgen_unit_conversion(0);kollmorgen_move(2);printf("执行命令:关节2反向旋转-%s\n",command);}
		else if(strncmp(command,"qq3",3)==0) {TargetVelocity_ph[3]= -1;kollmorgen_unit_conversion(0);kollmorgen_move(3);printf("执行命令:关节3反向旋转-%s\n",command);}
		else if(strncmp(command,"qq4",3)==0) {TargetVelocity_ph[4]= -1;kollmorgen_unit_conversion(0);kollmorgen_move(4);printf("执行命令:关节4反向旋转-%s\n",command);}
		else if(strncmp(command,"qq5",3)==0) {TargetVelocity_ph[5]= -1;kollmorgen_unit_conversion(0);kollmorgen_move(5);printf("执行命令:关节5反向旋转-%s\n",command);}
		else if(strncmp(command,"qq6",3)==0) {TargetVelocity_ph[6]= -1;kollmorgen_unit_conversion(0);kollmorgen_move(6);printf("执行命令:关节6反向旋转-%s\n",command);}
		

		else if(strncmp(command,"z1",2)==0){move_stop(1);printf("执行命令:关节停止-%s\n",command);}
		else if(strncmp(command,"z2",2)==0){move_stop(2);printf("执行命令:关节停止-%s\n",command);}
		else if(strncmp(command,"z3",2)==0){move_stop(3);printf("执行命令:关节停止-%s\n",command);}
		else if(strncmp(command,"z4",2)==0){move_stop(4);printf("执行命令:关节停止-%s\n",command);}
		else if(strncmp(command,"z5",2)==0){EnterMutex();TargetVelocity2=0;LeaveMutex();} 				
		
		//{move_stop(5);printf("执行命令:关节停止-%s\n",command);}
		else if(strncmp(command,"z6",2)==0){move_stop(6);printf("执行命令:关节停止-%s\n",command);}

		else if(strncmp(command,"zz",2)==0)
		{
		    printf("执行命令:全部结束-%s\n",command);
			move_stop(0);
	    //   TargetVelocity_ph[1]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(1);
		//   TargetVelocity_ph[2]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(2);
		//   TargetVelocity_ph[3]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(3);
		//   TargetVelocity_ph[4]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(4);
		//   TargetVelocity_ph[5]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(5);
		//   TargetVelocity_ph[6]= 0;kollmorgen_unit_conversion(0);kollmorgen_move(6);
		}
		else if(strncmp(command,"hh",2)==0)
		{
			OperationMode1=0x04;
			OperationMode2=0x04;
			OperationMode3=0x04;
			OperationMode4=0x04;
			OperationMode5=0x04;
			OperationMode6=0x04;
		}
		else
		{
			printf("错误命令:%s\n",command);
		}
	}
}
void kollmorgen_teach()
{
	char command[200];
	
	while(1)
	{
		EnterMutex();
		OperationMode1 = 0x04;
		OperationMode2 = 0x04;
		OperationMode3 = 0x04;
		OperationMode4 = 0x04;
		OperationMode5 = 0x04;
		OperationMode6 = 0x04;
		LeaveMutex();
	    system("clear");
		kollmorgen_unit_conversion(2);
		printf("********************************\n");
		printf("*         示教模式              *\n");
		printf("*group0[%3.0lf %3.0lf %3.0lf %3.0lf %3.0lf %3.0lf]*\n",group0_ph[1],group0_ph[2],group0_ph[3],group0_ph[4],group0_ph[5],group0_ph[6]);
		printf("*group1[%3.0lf %3.0lf %3.0lf %3.0lf %3.0lf %3.0lf]*\n",group1_ph[1],group1_ph[2],group1_ph[3],group1_ph[4],group1_ph[5],group1_ph[6]);
		printf("*group2[%3.0lf %3.0lf %3.0lf %3.0lf %3.0lf %3.0lf]*\n",group2_ph[1],group2_ph[2],group2_ph[3],group2_ph[4],group2_ph[5],group2_ph[6]);		
		printf("*    b0  b1 b2 储存两组动作组   *\n");
		printf("*        监控[check/c]         *\n");
		printf("*    全停[]   退出[exit/e]     *\n");
		printf("*******************************\n");
		printf("请输入指令:");
	    fgets(command, sizeof(command), stdin);
	    if(strncmp(command,"exit",4)==0 ||strncmp(command,"e",1)==0)
		{
			printf("结束测试模式\n");
			break;
		}
		else if(strncmp(command,"check",5)==0|| strncmp(command,"c",1)==0){kollmorgen_check();}
		else if(strncmp(command,"b0",2)==0)
		{
			EnterMutex();
			group0[1]=ActualPosition6;
			group0[2]=ActualPosition5;
			group0[3]=ActualPosition4;
			group0[4]=ActualPosition3;
			group0[5]=ActualPosition2;
			group0[6]=ActualPosition1;
			LeaveMutex();
		}
		else if(strncmp(command,"b1",2)==0)
		{
			EnterMutex();
			group1[1]=ActualPosition6;
			group1[2]=ActualPosition5;
			group1[3]=ActualPosition4;
			group1[4]=ActualPosition3;
			group1[5]=ActualPosition2;
			group1[6]=ActualPosition1;
			LeaveMutex();
		}
		else if(strncmp(command,"b2",2)==0)
		{
			EnterMutex();
			group2[1]=ActualPosition6;
			group2[2]=ActualPosition5;
			group2[3]=ActualPosition4;
			group2[4]=ActualPosition3;
			group2[5]=ActualPosition2;
			group2[6]=ActualPosition1;
			LeaveMutex();
			
		}
        else if(strncmp(command,"run",3)==0 || strncmp(command,"r",1)==0)
		{
            kollmorgen_teach_2();	
			printf("结束测试模式3\n");
			EnterMutex();
			TargetPosition1=ActualPosition1;
			TargetPosition2=ActualPosition2;
			TargetPosition3=ActualPosition3;
			TargetPosition4=ActualPosition4;
			TargetPosition5=ActualPosition5;
			TargetVelocity6=0;
			LeaveMutex();
			printf("结束测试模式4\n");
		}
		
		else
		{
			printf("错误命令:%s\n",command);
		}
	}

}

int kollmorgen_teach_2()
{
	EnterMutex();
	OperationMode1 = 0x01;
	OperationMode2 = 0x01;
	OperationMode3 = 0x01;
	OperationMode4 = 0x01;
	OperationMode5 = 0x01;
	OperationMode6 = 0x03;
	TargetPosition1=ActualPosition1;
	TargetPosition2=ActualPosition2;
	TargetPosition3=ActualPosition3;
	TargetPosition4=ActualPosition4;
	TargetPosition5=ActualPosition5;
	TargetVelocity6=0;
	LeaveMutex();

	char command[200];
	while(1)
	{
	     system("clear");
	    kollmorgen_unit_conversion(2);
		printf("********************************\n");
		printf("*      示教运动模式(2)          *\n");
		printf("*group0[%3.0lf %3.0lf %3.0lf %3.0lf %3.0lf %3.0lf]*\n",group0_ph[1],group0_ph[2],group0_ph[3],group0_ph[4],group0_ph[5],group0_ph[6]);
		printf("*group1[%3.0lf %3.0lf %3.0lf %3.0lf %3.0lf %3.0lf]*\n",group1_ph[1],group1_ph[2],group1_ph[3],group1_ph[4],group1_ph[5],group1_ph[6]);
		printf("*group2[%3.0lf %3.0lf %3.0lf %3.0lf %3.0lf %3.0lf]*\n",group2_ph[1],group2_ph[2],group2_ph[3],group2_ph[4],group2_ph[5],group2_ph[6]);		
		printf("*    r0  r1 r2 执行两组动作组   *\n");
		printf("*        监控[check/c]         *\n");
		printf("*    全停[]   退出[exit/e]     *\n");
		printf("*******************************\n");

		// printf("准备输入位置0:%d\n",group0[1]);
		// printf("准备输入位置1:%d\n",group1[1]);
		// printf("准备输入位置2:%d\n",group2[1]);
		// printf("目标位置:%lf\n",TargetPosition[1]);
		// printf("物理位置:%lf\n",group0_ph[1]);

		printf("进入运动模式-请输入指令:");
		
		fgets(command, sizeof(command), stdin);
		
		if(strncmp(command,"exit",4)==0 ||strncmp(command,"e",1)==0)
		{
			printf("结束测试模式\n");
			break;
		}
		else if(strncmp(command,"r0",2)==0)
		{
			TargetPosition[1]=group0[1]+0.0;pos_control_invel=1;
			TargetPosition[2]= group0[2];kollmorgen_move(2);
			TargetPosition[3]= group0[3];kollmorgen_move(3);
			TargetPosition[4]= group0[4];kollmorgen_move(4);
			TargetPosition[5]= group0[5];kollmorgen_move(5);
			TargetPosition[6]= group0[6];kollmorgen_move(6);
		}
		else if(strncmp(command,"r1",2)==0)
		{
			TargetPosition[1]=group1[1]+0.0;pos_control_invel=1;

			TargetPosition[2]= group1[2];kollmorgen_move(2);
			TargetPosition[3]= group1[3];kollmorgen_move(3);
			TargetPosition[4]= group1[4];kollmorgen_move(4);
			TargetPosition[5]= group1[5];kollmorgen_move(5);
			TargetPosition[6]= group1[6];kollmorgen_move(6);
		}
		else if(strncmp(command,"r2",2)==0)
		{
			TargetPosition[1]=group2[1]+0.0;pos_control_invel=1;

			TargetPosition[2]= group2[2];kollmorgen_move(2);
			TargetPosition[3]= group2[3];kollmorgen_move(3);
			TargetPosition[4]= group2[4];kollmorgen_move(4);
			TargetPosition[5]= group2[5];kollmorgen_move(5);
			TargetPosition[6]= group2[6];kollmorgen_move(6);
		}
	}
    printf("结束测试模式2\n");
	

}

int kollmorgen_move( int joint)
{
    switch (joint)
	{
		EnterMutex();
		case 1: switch (OperationMode6) {case 1: TargetPosition6=TargetPosition[1];break; case 3: TargetVelocity6=TargetVelocity[1];break; case 4: TargetTorque6=TargetTorque[1];break; } break;
		case 2: switch (OperationMode5) {case 1: TargetPosition5=TargetPosition[2];break; case 3: TargetVelocity5=TargetVelocity[2];break; case 4: TargetTorque5=TargetTorque[2];break; } break;
		case 3: switch (OperationMode4) {case 1: TargetPosition4=TargetPosition[3];break; case 3: TargetVelocity4=TargetVelocity[3];break; case 4: TargetTorque4=TargetTorque[3];break; } break;
		case 4: switch (OperationMode3) {case 1: TargetPosition3=TargetPosition[4];break; case 3: TargetVelocity3=TargetVelocity[4];break; case 4: TargetTorque3=TargetTorque[4];break; } break;
		case 5: switch (OperationMode2) {case 1: TargetPosition2=TargetPosition[5];break; case 3: TargetVelocity2=TargetVelocity[5];break; case 4: TargetTorque2=TargetTorque[5];break; } break;	
		case 6: switch (OperationMode1) {case 1: TargetPosition1=TargetPosition[6];break; case 3: TargetVelocity1=TargetVelocity[6];break; case 4: TargetTorque1=TargetTorque[6];break; } break;
		LeaveMutex();
	}
	return 0;
}
int Rgm6_Start()
{
	StartTimerLoop(&InitNodes);
	return 0;
}

int Rgm6_Stop()
{
	ControlWord = SHUTDOWN;
	StopTimerLoop(&Exit);
    TimerCleanup();
    printf("Finished RGM6 kollmorgen process\n");
	
	
	return 0;
}


int joint_1_move()
{
	{
		if(pidh==0)
		{
			
			PID_init();
			
			// printf("请输入[Kp Kd Ki]参数:\n");
			// scanf("%f %f %f",&pid1.Kp,&pid1.Kd,&pid1.Ki);
			// printf("Kp=%f,Kd=%f,Ki=%f\n",pid1.Kp,pid1.Kd,pid1.Ki);

			pidh=1;
			pidcount=0;
		}
		switch (pos_control_invel)
		{
		    case 1:pos_control_invel_move(1);break;
		}
	}
}


/*******************************************
*          Position                 *
*******************************************/


void fourier_tra_pos4(double t,double p[])
 {
  double t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19;
  double t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39;
  double t40,t41,t42,t43,t44,t45;
      t2 = t*t;
  t3 = t*t*t;
  t5 = t*t*t*t*t;
  t6 = 1.0/3.141592653589793;
  t7 = (t*3.141592653589793)/2.0;
  t8 = (t*3.141592653589793)/5.0;
  t9 = t*3.141592653589793*(2.0/5.0);
  t10 = (t*3.141592653589793)/1.0E+1;
  t11 = t*3.141592653589793*(3.0/1.0E+1);
  t4 = t2*t2;
  t12 = cos(t7);
  t13 = cos(t8);
  t14 = cos(t9);
  t15 = sin(t7);
  t16 = sin(t8);
  t17 = sin(t9);
  t18 = cos(t10);
  t19 = cos(t11);
  t20 = sin(t10);
  t21 = sin(t11);
  p[0] = t*(-3.224931412063841)-t2*1.868518541246288+t3*2.674751394262247E-1-t4*1.071804275073542E-2+t5*1.20934927952394E-4+t6*t12*6.693065784261684+t6*t13*7.06929707000894E-1-t6*t14*1.091487357313827E+1+t6*t15*4.450838148065941E-1+t6*t16*1.372980510190052E+1-t6*t17*2.05939093137836-t6*t18*6.407314223884445E+1-t6*t19*5.599647651641183+t6*t20*1.757074153715141E+1-t6*t21*2.25629765627786+2.329635826233939E+1;
  p[1] = t*(-4.98710891479843)+t2*1.92616911539645-t3*6.793918866968425E-2-t4*4.535406426755933E-3+t5*1.870165843049413E-4-t6*t12*1.532398346127331E+1+t6*t13*2.868568504883934E+1+t6*t14*2.153419109611018E+1-t6*t15*1.203502012271384E+1+t6*t16*2.766394867719378E+1-t6*t17*8.294222583155135+t6*t18*4.235468344713762E+1+t6*t19*4.532044144544874E-1+t6*t20*5.666724697964172E+1+t6*t21*1.040931192004826E+1+6.52661184585854E+1;
  p[2] = t*6.177211514579021-t2*2.838166930236752+t3*1.293864051591997E-1+t4*4.486854264243781E-3-t5*2.316454317967133E-4+t6*t12*5.01534503747691-t6*t13*2.826003706406154E+1+t6*t14*3.641879035482938+t6*t15*6.59117375465177-t6*t16*2.287379023479015E+1+t6*t17*1.635357679042727E+1-t6*t18*6.186968658316211E+1-t6*t19*2.104746379689926E+1-t6*t20*7.530760781356456E+1-t6*t21*1.302903426587109E+1+1.226331178722413E+2;
  p[3] = t*6.968914747486057-t2*7.898499024856598E-1-t3*9.523787843858546E-2+t4*1.10920903953222E-2-t5*2.613343030307271E-4+t6*t12*5.965339537992506-t6*t13*1.084384361564073E+1-t6*t14*2.162412513312796E+1+t6*t15*4.344060769035011-t6*t16*7.450277253380353-t6*t17*8.476992097723985-t6*t18*6.122073053813239E+1+t6*t19*2.790724579649249E+1-t6*t20*7.840108829469012E+1+t6*t21*1.193338662410372E+1+1.090400604241502E+2;
  p[4] = t*5.944306455714509-t2*1.618541970564746+t3*1.324653566361189E-2+t4*7.099219678052838E-3-t5*2.229114920892942E-4+t6*t12*6.813535001110127-t6*t13*2.298107598968429E+1-t6*t14*4.811110824059934+t6*t15*1.437949157457616E-1+t6*t16*9.678122703568706+t6*t17*8.889122331497967-t6*t18*6.638822349140348E+1-t6*t19*4.231961831053794-t6*t20*8.878107914038093E+1-t6*t21*8.764564909540749+2.915681542303851E+1;
  p[5] = t*(-9.888491058369965)-t2*1.58536124806925+t3*4.057484012661741E-1-t4*2.250432385461681E-2+t5*3.708184146888737E-4-t6*t12*5.200036093452777+t6*t13*8.287897731961733+t6*t14*6.46067036245606+t6*t15*2.63027388606605+t6*t16*1.09200000762982E+1-t6*t17*7.682425799785664E-1-t6*t18*4.896142624132867E+1-t6*t19*6.498579981790987+t6*t20*7.732242524830482E+1-t6*t21*3.451971309205852+1.461407613418409E+1;
 }


void fourier_tra_pos3(double t,double p[])
 {
  double t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19;
  double t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39;
  double t40,t41,t42,t43,t44,t45;

  
  t2 = t*3.141592653589793;
  t3 = t*t;
  t4 = t*t*t;
  t6 = t*t*t*t*t;
  t9 = 1.0/3.141592653589793;
  t5 = t3*t3;
  t7 = cos(t2);
  t8 = sin(t2);
  t10 = t2/5.0;
  t11 = t2*(2.0/5.0);
  t12 = t2*(3.0/5.0);
  t13 = t2*(4.0/5.0);
  t14 = cos(t10);
  t15 = cos(t11);
  t16 = cos(t12);
  t17 = cos(t13);
  t18 = sin(t10);
  t19 = sin(t11);
  t20 = sin(t12);
  t21 = sin(t13);
  p[0] = t*3.413150768014772+t3*5.32786752249123-t4*1.406888581299723+t5*1.044759367451339E-1-t6*2.047890460808864E-3+t7*t9*3.455511492714479-t8*t9*1.130134514889162+t9*t14*4.811516729275876E+1-t9*t15*2.58357023429177+t9*t16*4.568256234565286-t9*t17*5.030458409388519-t9*t18*1.153771049478816E+1-t9*t19*7.175536188890219-t9*t20*6.943304428155243E-1+t9*t21*4.13917323384678-1.544595742573769E+1;
  p[1] = t*(-2.219748534300089)+t3*1.246157757780389-t4*2.725669812606892E-2-t5*2.083465043669744E-2+t6*1.331849120580053E-3-t7*t9*2.075545709010996-t8*t9*3.736970847707179+t9*t14*1.803507822704177E+1-t9*t15*1.502508527565549-t9*t16*5.263672665027768+t9*t17*6.691866831003213+t9*t18*3.919217601575307-t9*t19*2.086075457797265+t9*t20*3.055005255701696E-1+t9*t21*7.280007161836264+8.494357801661869E+1;
  p[2] = t*(-7.056968139019869)-t3*9.270447182577108+t4*2.559786250417408-t5*1.985589939110691E-1+t6*4.234180883411922E-3+t7*t9*2.905832166127304+t8*t9*2.558084531549056-t9*t14*3.73962926272505E+1+t9*t15*6.569110633380175-t9*t16*1.04526587962506E+1-t9*t17*7.187235891946595+t9*t18*1.554374021089798E+1+t9*t19*1.322598877418446E+1-t9*t20*1.008573819733058E+1+t9*t21*2.68897871751973+1.045025945562608E+2;
  p[3] = t*1.166183551705395E+1+t3*9.234968556016-t4*3.013177262908595+t5*2.672772183159693E-1-t6*6.997101310232373E-3-t7*t9*1.612500250806139+t8*t9*6.672207308008673E-1+t9*t14*2.935834435623246E+1-t9*t15*1.031820253894722E+1+t9*t16*9.263042379220251+t9*t17*7.239917509721538-t9*t18*1.636136789642065E+1-t9*t19*2.464338426898177E+1+t9*t20*1.057656449081633E+1-t9*t21*6.931709569334723+7.919955411257742E+1;
  p[4] = t*(-7.27717442895332)+t3*5.334726526018159-t4*3.392278623082993E-1-t5*5.58103511741182E-2+t6*4.366304657371993E-3+t7*t9*3.519261125671957-t8*t9*6.032645277628181+t9*t14*4.743634840083173E+1-t9*t15*1.22178424214694E+1+t9*t16*3.446736219866002-t9*t17*2.041395198101414+t9*t18*4.487730951700581E+1-t9*t19*1.493943346147892+t9*t20*3.81693179475224+t9*t21*3.302220080985192-1.277794817890495E+1;
  p[5] = t*1.064078106871648E+1+t3*4.103054534278705-t4*1.884689013727389+t5*2.006422613735343E-1-t6*6.38446864122989E-3-t7*t9*7.702323570463122-t8*t9*1.050139843797545+t9*t14*3.494807386128718E+1+t9*t15*8.704708832245589+t9*t16*4.193776699246269+t9*t17*9.396833179293698-t9*t18*4.870649926835751E+1+t9*t19*1.83237243238596-t9*t20*7.067579532537881+t9*t21*4.572821719151134-1.576941203532568E+1;
 
 }

void fourier_tra_pos2(double t,double p[])
 {
  double t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19;
  double t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39;
  double t40,t41,t42,t43,t44,t45;

    t2 = t*3.141592653589793;
  t3 = t*t;
  t4 = t*t*t;
  t6 = t*t*t*t*t;
  t9 = 1.0/3.141592653589793;
  t5 = t3*t3;
  t7 = cos(t2);
  t8 = sin(t2);
  t10 = t2/5.0;
  t11 = t2*(2.0/5.0);
  t12 = t2*(3.0/5.0);
  t13 = t2*(4.0/5.0);
  t14 = cos(t10);
  t15 = cos(t11);
  t16 = cos(t12);
  t17 = cos(t13);
  t18 = sin(t10);
  t19 = sin(t11);
  t20 = sin(t12);
  t21 = sin(t13);
  p[0] = t*8.560321612789311+t3*9.293874387283543E-1-t4*1.041909649024602+t5*1.376986985791232E-1-t6*5.136192967673586E-3+t7*t9*1.36490491228523E-1+t8*t9*2.461791979595574+t9*t14*1.21864684385627E+1+t9*t15*7.263872141352401E-1-t9*t16*7.880760301458517+t9*t17*4.200888976838955-t9*t18*3.765982172731643E+1-t9*t19*6.317316724090594E-1-t9*t20*1.455438230219261-t9*t21*2.955242049783022-2.982396463335473;
  p[1] = t*(-1.037901522304992E+1)+t3*4.203451122506658+t4*1.972112978036604E-1-t5*1.136507171206823E-1+t6*6.227409133829953E-3+t7*t9*3.744575559933239E-1-t8*t9*4.687375762720009+t9*t14*3.581136613727847E+1+t9*t15*1.115590145062947E+1-t9*t16*4.852245881943293+t9*t17*1.298362782316955+t9*t18*1.936604377802885E+1+t9*t19*1.698950196352191E+1-t9*t20*3.34609267294077E-1+t9*t21*5.747683756414801+7.606189698265305E+1;
  p[2] = t*3.941079192049486-t3*5.325460345947926+t4*6.709841499846368E-1+t5*5.861584421263032E-3-t6*2.364647515229692E-3-t7*t9*7.995472390566217+t8*t9*1.113007651609898-t9*t14*2.227938461805021E+1-t9*t15*7.315097867532776+t9*t16*6.801505432975774+t9*t17*6.590981445362878+t9*t18*1.356484258514189E+1-t9*t19*3.67634868753056+t9*t20*5.784279112969104-t9*t21*1.220885419182125E+1+9.770229328431899E+1;
  p[3] = t*5.151204955239516+t3*4.186859546502989-t4*1.352492404824549+t5*1.191366697936226E-1-t6*3.09072297314371E-3+t7*t9*8.812248493954169+t8*t9*1.855395458896727+t9*t14*2.825568147263894E+1+t9*t15*5.250846542690653-t9*t16*2.537835685685511-t9*t17*1.125555122095887E+1-t9*t18*1.736117990875693E+1-t9*t19*7.707612193480806-t9*t20*8.866590275691499+t9*t21*6.085793263027957+8.092008648223557E+1;
  p[4] = t*2.80161048025669+t3*3.212804527488838-t4*9.227219535234362E-1+t5*7.41522024787387E-2-t6*1.680966288154014E-3+t7*t9*5.711307766612492E-1+t8*t9*1.16030956653184+t9*t14*2.648467271306405E+1-t9*t15*2.735141595584123-t9*t16*3.599283570202016+t9*t17*3.356534284896645-t9*t18*6.231090192443141+t9*t19*1.720275774475741-t9*t20*3.125336779573373-t9*t21*1.910762812932718-7.664237622061784;
  p[5] = t*(-9.44120089553952)+t3*8.951811271271517E-1+t4*7.650838641285217E-1-t5*1.326662021618213E-1+t6*5.664720537323712E-3-t7*t9*1.838389373336291E-2-t8*t9*2.979203338183956-t9*t14*1.913761765292223+t9*t15*3.728166483534245E-3-t9*t16*2.079801435806283+t9*t17*2.207744343714673+t9*t18*3.814914663013388E+1+t9*t19*1.127202193259606E+1-t9*t20*4.223811072522565E-1+t9*t21*6.689934987620361E-1+5.731088601115491E-1;
 }

void fourier_tra_pos1(double t,double p[])
 {
  double t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19;
  double t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39;
  double t40,t41,t42,t43,t44,t45;

  t2 = 1.0/3.141592653589793;
  t3 = t*3.141592653589793;
  t4 = t*3.141592653589793*(1.0/5.0);
  t5 = t*3.141592653589793*(2.0/5.0);
  t6 = t*3.141592653589793*(3.0/5.0);
  t7 = t*3.141592653589793*(4.0/5.0);
  t8 = t*t;
  t9 = t8*t8;
  t10 = cos(t3);
  t11 = cos(t4);
  t12 = cos(t5);
  t13 = cos(t6);
  t14 = cos(t7);
  t15 = sin(t3);
  t16 = sin(t4);
  t17 = sin(t5);
  t18 = sin(t6);
  t19 = sin(t7);
  p[0] = t*(-1.269284191636659E1)-t8*1.415144842310716E1-t9*3.319071129765705E-1+t*t8*4.099573876258091+t*t9*7.615705149819954E-3+t2*t10*2.097898408684882-t2*t11*2.566714966259575E1+t2*t12*2.589509109816197E1-t2*t13*6.635368887084005-t2*t14*1.849185323801058E1+t2*t15*1.454746534809246E1-t2*t16*6.616460156282042E1+t2*t17*1.603709744838878E1+t2*t18*1.860042790419576E1-t2*t19*7.74599855129344+7.257905398648393;
  p[1] = t*(-2.834393751471628)-t8*1.006783474100025E1-t9*1.43194253682077E-1+t*t8*2.297006323347214+t*t9*1.700636250882977E-3+t2*t10*3.590234013770829+t2*t11*3.803726338651687E1-t2*t12*1.437699410457977E1-t2*t13*4.157741352195223-t2*t14*1.20687519793101E1-t2*t15*1.089518031494607E1+t2*t16*5.186704704485416E1-t2*t17*2.488212831732892E1+t2*t18*1.106136439605156E1+t2*t19*8.340246683434369+8.649094864300569E1;
  p[2] = t*2.955409196245879E1-t8*1.50566033965757E1+t9*2.927453454711249E-1+t*t8*5.59114830692613E-2-t*t9*1.773245517747528E-2-t2*t10*1.061533886701647E1+t2*t11*6.917456863351296E1-t2*t12*7.239825699783546-t2*t13*1.184944105317019E1+t2*t14*5.76124310285252-t2*t15*9.297098975196164-t2*t16*7.310549981446426E1+t2*t17*1.314080592351767E1-t2*t18*3.002339347645444-t2*t19*1.136351473148697E1+7.560245992913464E1;
  p[3] = t*2.391740206683298E1-t8*8.056777670665952+t9*2.781932542958352E-1-t*t8*7.803846725501076E-1-t*t9*1.435044124009979E-2-t2*t10*3.387058404074434-t2*t11*3.94931898316659E1+t2*t12*2.271914023997128E1-t2*t13*6.279134836366662-t2*t14*2.401393541684231-t2*t15*1.358094456035473E1+t2*t16*6.893693208691923E1-t2*t17*3.688821059896746E1-t2*t18*4.964262273165542-t2*t19*7.987502900469734+9.91805779915049E1;
  p[4] = t*1.301151874488344E1+t8*1.496596150340772E1+t9*3.448323962073289E-1-t*t8*4.294344175169889-t*t9*7.806911246930065E-3+t2*t10*9.138818145099208+t2*t11*3.667771411760958E1-t2*t12*1.633911677175625E1-t2*t13*1.562491055924178E1+t2*t14*1.118894809463415E1-t2*t15*1.163030614553412E1+t2*t16*2.435342038413799E1-t2*t17*1.507381750813793E1+t2*t18*3.587835857474045-t2*t19*2.968838984257718-7.97094206269259;
  p[5] = t*3.287248408876359E1-t8*2.042182622085385E1+t9*2.888689991229154E-1+t*t8*7.971168352944115E-1-t*t9*1.972349045325816E-2-t2*t10*3.925736474522484+t2*t11*1.907004711598127E1-t2*t12*3.320849852458219E1-t2*t13*9.350073692570907-t2*t14*1.810339807669332-t2*t15*9.147841576091849-t2*t16*3.43735302196679E1-t2*t17*3.566440332904707E1+t2*t18*1.254743580770411-t2*t19*4.171276606976984+9.302479540105132;
 
 
 }




/*******************************************
*           Velocity                   *
*                                     * 
*******************************************/
void fourier_tra_vel(double t,double pd[])
 {
  double t0,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18;
//   t=t-(int(t)/10)*10;
  t2 = t*3.141592653589793;
  t3 = t*3.141592653589793*(1.0/5.0);
  t4 = t*3.141592653589793*(2.0/5.0);
  t5 = t*3.141592653589793*(3.0/5.0);
  t6 = t*3.141592653589793*(4.0/5.0);
  t7 = t*t;
  t8 = cos(t2);
  t9 = cos(t3);
  t10 = cos(t4);
  t11 = cos(t5);
  t12 = cos(t6);
  t13 = sin(t2);
  t14 = sin(t3);
  t15 = sin(t4);
  t16 = sin(t5);
  t17 = sin(t6);
  t18 = t7*t7;
  pd[0] = t*(-2.830289684621432E1)+t7*1.229872162877427E1+t8*1.454746534809246E1-t9*1.323292031256408E1+t10*6.414838979355512+t11*1.116025674251745E1-t12*6.196798841034752-t13*2.097898408684882+t14*5.133429932519149-t15*1.035803643926479E1+t16*3.981221332250403+t17*1.479348259040847E1+t18*3.807852574909977E-2-t*t7*1.327628451906282-1.269284191636659E1;
  pd[1] = t*(-2.01356694820005E1)+t7*6.891018970041641-t8*1.089518031494607E1+t9*1.037340940897083E1-t10*9.952851326931567+t11*6.636818637630935+t12*6.672197346747495-t13*3.590234013770829-t14*7.607452677303374+t15*5.750797641831909+t16*2.494644811317134+t17*9.655001583448083+t18*8.503181254414887E-3-t*t7*5.727770147283079E-1-2.834393751471628;
  pd[2] = t*(-3.011320679315141E1)+t7*1.677344492077839E-1-t8*9.297098975196164-t9*1.462109996289285E1+t10*5.256322369407069-t11*1.801403608587266-t12*9.090811785189578+t13*1.061533886701647E1-t14*1.383491372670259E1+t15*2.895930279913419+t16*7.109664631902113-t17*4.608994482282016-t18*8.866227588737639E-2+t*t7*1.1709813818845+2.955409196245879E1;
  pd[3] = t*(-1.61135553413319E1)-t7*2.341154017650323-t8*1.358094456035473E1+t9*1.378738641738385E1-t10*1.475528423958698E1-t11*2.978557363899325-t12*6.390002320375787+t13*3.387058404074434+t14*7.89863796633318-t15*9.087656095988514+t16*3.767480901819997+t17*1.921114833347385-t18*7.175220620049895E-2+t*t7*1.112773017183341+2.391740206683298E1;
  pd[4] = t*2.993192300681544E1-t7*1.288303252550967E1-t8*1.163030614553412E1+t9*4.870684076827597-t10*6.029527003255174+t11*2.152701514484427-t12*2.375071187406174-t13*9.138818145099208-t14*7.335542823521917+t15*6.535646708702501+t16*9.374946335545065-t17*8.95115847570732-t18*3.903455623465032E-2+t*t7*1.379329584829315+1.301151874488344E1;
  pd[5] = t*(-4.08436524417077E1)+t7*2.391350505883234-t8*9.147841576091849-t9*6.87470604393358-t10*1.426576133161883E1+t11*7.528461484622468E-1-t12*3.337021285581587+t13*3.925736474522484-t14*3.814009423196254+t15*1.328339940983287E1+t16*5.610044215542544+t17*1.448271846135466-t18*9.861745226629078E-2+t*t7*1.155475996491662+3.287248408876359E1;

}
/*******************************************
*             acceleration           *
*                                    * 
*******************************************/
void fourier_tra_acc(double t,double pdd[])
 {
  double t0,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17;
//   t=t-(int(t)/10)*10;
  t2 = t*3.141592653589793;
  t3 = t*3.141592653589793*(1.0/5.0);
  t4 = t*3.141592653589793*(2.0/5.0);
  t5 = t*3.141592653589793*(3.0/5.0);
  t6 = t*3.141592653589793*(4.0/5.0);
  t7 = t*t;
  t8 = cos(t2);
  t9 = cos(t3);
  t10 = cos(t4);
  t11 = cos(t5);
  t12 = cos(t6);
  t13 = sin(t2);
  t14 = sin(t3);
  t15 = sin(t4);
  t16 = sin(t5);
  t17 = sin(t6);
  pdd[0] = t*2.459744325754855E1-t7*3.982885355718846-t8*3.141592653589793*2.097898408684882+t9*3.141592653589793*1.02668598650383-t10*3.141592653589793*4.143214575705915+t11*3.141592653589793*2.388732799350242+t12*3.141592653589793*1.183478607232677E1-t13*3.141592653589793*1.454746534809246E1+t14*3.141592653589793*2.646584062512817-t15*3.141592653589793*2.565935591742205-t16*3.141592653589793*6.696154045510472+t17*3.141592653589793*4.957439072827802+t*t7*1.523141029963991E-1-2.830289684621432E1;
  pdd[1] = t*1.378203794008328E1-t7*1.718331044184924-t8*3.141592653589793*3.590234013770829-t9*3.141592653589793*1.521490535460675+t10*3.141592653589793*2.300319056732764+t11*3.141592653589793*1.49678688679028+t12*3.141592653589793*7.724001266758466+t13*3.141592653589793*1.089518031494607E1-t14*3.141592653589793*2.074681881794167+t15*3.141592653589793*3.981140530772627-t16*3.141592653589793*3.982091182578561-t17*3.141592653589793*5.337757877397996+t*t7*3.401272501765955E-2-2.01356694820005E1;
  pdd[2] = t*3.354688984155678E-1+t7*3.512944145653499+t8*3.141592653589793*1.061533886701647E1-t9*3.141592653589793*2.766982745340518+t10*3.141592653589793*1.158372111965367+t11*3.141592653589793*4.265798779141268-t12*3.141592653589793*3.687195585825613+t13*3.141592653589793*9.297098975196164+t14*3.141592653589793*2.924219992578571-t15*3.141592653589793*2.102528947762828+t16*3.141592653589793*1.08084216515236+t17*3.141592653589793*7.272649428151662-t*t7*3.546491035495056E-1-3.011320679315141E1;
  pdd[3] = t*(-4.682308035300645)+t7*3.338319051550022+t8*3.141592653589793*3.387058404074434+t9*3.141592653589793*1.579727593266636-t10*3.141592653589793*3.635062438395406+t11*3.141592653589793*2.260488541091998+t12*3.141592653589793*1.536891866677908+t13*3.141592653589793*1.358094456035473E1-t14*3.141592653589793*2.757477283476769+t15*3.141592653589793*5.902113695834794+t16*3.141592653589793*1.787134418339595+t17*3.141592653589793*5.11200185630063-t*t7*2.870088248019958E-1-1.61135553413319E1;
  pdd[4] = t*(-2.576606505101933E1)+t7*4.137988754487946-t8*3.141592653589793*9.138818145099208-t9*3.141592653589793*1.467108564704383+t10*3.141592653589793*2.614258683481+t11*3.141592653589793*5.624967801327039-t12*3.141592653589793*7.160926780565856+t13*3.141592653589793*1.163030614553412E1-t14*3.141592653589793*9.741368153655195E-1+t15*3.141592653589793*2.41181080130207-t16*3.141592653589793*1.291620908690656+t17*3.141592653589793*1.900056949924939-t*t7*1.561382249386013E-1+2.993192300681544E1;
  pdd[5] = t*4.782701011766469+t7*3.466427989474985+t8*3.141592653589793*3.925736474522484-t9*3.141592653589793*7.628018846392507E-1+t10*3.141592653589793*5.31335976393315+t11*3.141592653589793*3.366026529325526+t12*3.141592653589793*1.158617476908373+t13*3.141592653589793*9.147841576091849+t14*3.141592653589793*1.374941208786716+t15*3.141592653589793*5.70630453264753-t16*3.141592653589793*4.517076890773481E-1+t17*3.141592653589793*2.669617028465269-t*t7*3.944698090651631E-1-4.08436524417077E1;

 }
 
 /*******************************************
* gravity_compensation                 
* para@(p[],gra_compen[])
*******************************************/
void gravity_compensation(double p[],double gra_compen[]) 
{
  double q1,q2,q3,q4,q5,q6;
  q1=p[1]*pi/180;
  q2=p[2]*pi/180;
  q3=p[3]*pi/180;
  q4=p[4]*pi/180;
  q5=p[5]*pi/180;
  q6=p[6]*pi/180;
  double t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21;
  double t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39,t40,t41;
  double t42,t43,t44,t45,t46,t47,t48,t49,t50,t51,t52,t53,t54,t55,t56,t57,t58,t59,t60,t61;
  double t62,t63,t64,t65,t66,t67,t68,t69,t70,t71,t72,t73,t74,t75,t76,t77,t78,t79,t80,t81;
  double t82,t83,t84,t85,t86,t87,t88,t89,t90,t91,t92,t93,t94,t95,t96,t97,t98,t99,t100;
  double t101,t102,t103,t104,t105,t106,t107,t108,t109,t110,t111,t112,t113,t114,t115,t116,t117,t118,t119,t120;
  double t121,t122,t123,t124,t125,t126,t127,t128,t129,t130,t131,t132,t133,t134,t135,t136,t137,t138,t139,t140;
  double t141,t142,t143,t144,t145,t146,t147,t148,t149,t150,t151,t152,t153,t154,t155,t156,t157,t158,t159,t160,t161;
  t2 = cos(q5);
  t3 = cos(q4);
  t4 = cos(q2);
  t5 = sin(q3);
  t6 = cos(q3);
  t7 = sin(q2);
  t8 = cos(q6);
  t9 = t4*t6*9.8015;
  t15 = t5*t7*9.8015;
  t10 = t9-t15;
  t11 = sin(q4);
  t12 = t4*t5*9.8015;
  t13 = t6*t7*9.8015;
  t14 = t12+t13;
  t16 = t3*t10;
  t17 = t11*t14;
  t18 = sin(q5);
  t19 = t3*t14;
  t20 = t10*t11;
  t21 = t19+t20;
  t22 = t16-t17;
  t23 = sin(q6);
  t24 = t18*6.001687800921391E-16;
  t25 = t2*t21;
  t26 = t18*t22*6.123233995736766E-17;
  t27 = t24+t25+t26;
  t28 = t2*6.001687800921391E-16;
  t29 = t2*t22*6.123233995736766E-17;
  t33 = t18*t21;
  t30 = t28+t29-t33;
  t31 = -t16+t17+3.674973877440049E-32;
  t32 = t8*t27*(1.19E2/5.0E2);
  t34 = t23*t30*1.45732969098535E-17;
  t39 = t23*t31*(1.19E2/5.0E2);
  t35 = t32+t34-t39;
  t36 = t23*t27*(1.19E2/5.0E2);
  t37 = t8*t31*(1.19E2/5.0E2);
  t40 = t8*t30*1.45732969098535E-17;
  t38 = t36+t37-t40;
  t41 = t2*1.043093339800138E-15;
  t42 = t2*t22*1.06421806845905E-16;
  t43 = t23*t35*6.123233995736766E-17;
  t44 = t11*t14*1.45732969098535E-17;
  t56 = t8*t38*6.123233995736766E-17;
  t57 = t18*t21*(8.69E2/5.0E2);
  t58 = t3*t10*1.45732969098535E-17;
  t45 = t41+t42+t43+t44-t56-t57-t58+5.355648545188942E-49;
  t46 = t2*8.746437828307318E-33;
  t47 = t2*t22*8.923570706838053E-34;
  t48 = t8*t38;
  t49 = t18*9.002531701382087E-16;
  t50 = t2*t21*(3.0/2.0);
  t51 = t18*t22*9.184850993605149E-17;
  t52 = t8*t35;
  t53 = t23*t38;
  t54 = t49+t50+t51+t52+t53;
  t55 = t11*t14*3.0;
  t90 = t18*t21*1.45732969098535E-17;
  t91 = t23*t35;
  t92 = t18*t54*6.123233995736766E-17;
  t93 = t3*t10*3.0;
  t94 = t2*t45*6.123233995736766E-17;
  t59 = t46+t47+t48+t55-t90-t91-t92-t93-t94+5.512460816160074E-32;
  t60 = t2*1.085585289430661E-20;
  t61 = t2*t22*1.107570565148866E-21;
  t62 = t8*t27*5.253374E-3;
  t63 = t23*t30*3.216763826911964E-19;
  t64 = t11*t14*1.107570565148866E-21;
  t80 = t18*t21*1.8088E-5;
  t81 = t23*t31*5.253374E-3;
  t82 = t3*t10*1.107570565148866E-21;
  t65 = t60+t61+t62+t63+t64-t80-t81-t82+4.070292894343596E-53;
  t66 = t2*3.271039885258177E-20;
  t67 = t2*t22*3.337284992356452E-21;
  t68 = t8*t31*5.253374E-3;
  t69 = t11*t14*3.337284992356452E-21;
  t70 = t23*t27*5.253374E-3;
  t72 = t18*t21*5.4502E-5;
  t73 = t8*t30*3.216763826911964E-19;
  t74 = t3*t10*3.337284992356452E-21;
  t71 = t66+t67+t68+t69+t70-t72-t73-t74+1.226443516848268E-52;
  t75 = t8*t35*5.725223786013876E-18;
  t76 = t23*t38*5.725223786013876E-18;
  t77 = t8*t30*1.107570565148866E-21;
  t78 = t8*t27*5.4502E-5;
  t79 = t23*t30*3.337284992356452E-21;
  t100 = t8*t31*1.8088E-5;
  t101 = t23*t31*5.4502E-5;
  t102 = t23*t71*6.123233995736766E-17;
  t103 = t23*t27*1.8088E-5;
  t104 = t8*t65*6.123233995736766E-17;
  t83 = t75+t76+t77+t78+t79-t100-t101-t102-t103-t104;
  t84 = t2*5.017826918641907E-50;
  t85 = t23*t65;
  t86 = t2*t22*5.119447960661029E-51;
  t87 = t8*t38*9.35E-2;
  t88 = t11*t14*5.119447960661029E-51;
  t105 = t18*t21*8.360693000178317E-35;
  t106 = t23*t35*9.35E-2;
  t107 = t8*t71;
  t108 = t3*t10*5.119447960661029E-51;
  t89 = t84+t85+t86+t87+t88-t105-t106-t107-t108+1.881383752234302E-82;
  t95 = t2*1.379944572539562E-66;
  t96 = t2*t22*1.407891213119994E-67;
  t97 = t8*t38*1.577721810442024E-34;
  t98 = t18*t54*1.023E-1;
  t99 = t11*t14*2.366582715663035E-34;
  t109 = t2*t45*1.023E-1;
  t110 = t18*t83;
  t129 = t18*t21*2.299260838472322E-51;
  t130 = t23*t35*1.577721810442024E-34;
  t131 = t3*t10*2.366582715663035E-34;
  t132 = t2*t89;
  t111 = t95+t96+t97+t98+t99+t109+t110-t129-t130-t131-t132+8.697129658862787E-66;
  t112 = t8*t31*1.107570565148866E-21;
  t113 = t8*t35*9.35E-2;
  t114 = t23*t38*9.35E-2;
  t115 = t23*t31*3.337284992356452E-21;
  t116 = t2*t54*6.264068377638711E-18;
  t117 = t2*t83*6.123233995736766E-17;
  t118 = t23*t27*1.107570565148866E-21;
  t119 = t18*t89*6.123233995736766E-17;
  t134 = t18*t45*6.264068377638711E-18;
  t135 = t23*t71;
  t136 = t8*t30*6.781913737196921E-38;
  t137 = t8*t27*3.337284992356452E-21;
  t138 = t23*t30*2.043497691865914E-37;
  t139 = t8*t65;
  t120 = t112+t113+t114+t115+t116+t117+t118+t119-t134-t135-t136-t137-t138-t139;
  t121 = t2*t54;
  t122 = t3*t14*(3.0/2.0);
  t123 = t10*t11*(3.0/2.0);
  t125 = t18*t45;
  t124 = t121+t122+t123-t125;
  t126 = t2*1.972485359193087E-49;
  t127 = t2*t22*2.012432137114816E-50;
  t128 = t8*t38*2.255187080629851E-17;
  t133 = t18*t54*3.683E-1;
  t140 = t11*t14*3.382780620944776E-17;
  t141 = t2*t45*3.683E-1;
  t142 = t4*t6*3.4980534144E-1;
  t143 = t18*(t75+t76+t77+t78+t79-t100-t101-t102-t103-t104);
  t144 = t95+t96+t97+t98+t99+t109-t129-t130-t131-t132+t143+8.697129658862787E-66;
  t145 = t11*t120;
  t146 = t11*t124*1.173E-1;
  t147 = t4*t6*2.509184E1;
  t148 = t11*t124;
  t149 = t11*t59;
  t150 = t4*t5*2.509184E1;
  t151 = t6*t7*2.509184E1;
  t152 = t3*t124;
  t153 = t149+t150+t151+t152;
  t154 = t4*t6*6.19394579584;
  t155 = t2*t54*1.023E-1;
  t156 = t8*t30*4.152724475175836E-54;
  t157 = t11*t124*3.683E-1;
  t158 = t8*t27*2.043497691865914E-37;
  t159 = t23*t30*1.251281453704298E-53;
  t160 = t2*(t75+t76+t77+t78+t79-t100-t101-t102-t103-t104);
  t161 = t18*t89;
  gra_compen[0] = 0;
  gra_compen[1] = t4*7.471861244635095E-16+t119+t4*t6*3.79269794648383E-16-t5*t7*3.79269794648383E-16+t8*t27*1.251281453704298E-53+t8*t30*2.54281036813248E-70-t8*t31*4.152724475175836E-54-t8*t35*3.505688491972092E-34-t23*t27*4.152724475175836E-54+t23*t30*7.661889135557078E-70-t23*t31*1.251281453704298E-53+t2*t54*6.264068377638712E-18-t23*t38*3.505688491972092E-34-t3*t59*2.255187080629851E-17-t18*t45*6.264068377638712E-18+t8*t65*3.749399456654644E-33+t23*t71*3.749399456654644E-33+t11*t124*2.255187080629851E-17+t5*t153*2.459090772687885E-17+t2*(t75+t76+t77+t78+t79-t100-t101-t102-t103-t104)*6.123233995736766E-17-t7*(t4*7.28016214-t5*(t126+t127+t128+t133+t140+t141-t4*t5*3.4980534144E-1-t3*t10*3.382780620944776E-17-t6*t7*3.4980534144E-1-t18*t21*3.286551091328455E-34-t23*t35*2.255187080629851E-17-t11*t59*1.173E-1-t3*t120-t3*t124*1.173E-1+t11*t144+7.108330372102853E-16)+t6*(t142+t145+t146-t5*t7*3.4980534144E-1-t3*t59*1.173E-1+t3*t144))+t6*(t147+t148-t5*t7*2.509184E1-t3*t59)*2.459090772687885E-17-t4*(t2*2.150828455747879E-49-t7*7.28016214-t3*t10*3.688636159031828E-17+t2*t22*2.194387038461337E-50+t11*t14*3.688636159031828E-17-t18*t21*3.583705995866162E-34+t8*t38*2.459090772687885E-17+t2*t45*(2.51E2/6.25E2)-t23*t35*2.459090772687885E-17+t18*t54*(2.51E2/6.25E2)+t6*(t126+t127+t128+t133+t140+t141-t4*t5*3.4980534144E-1-t3*t10*3.382780620944776E-17-t6*t7*3.4980534144E-1-t18*t21*3.286551091328455E-34-t23*t35*2.255187080629851E-17-t11*t59*1.173E-1+t11*t111-t3*t120-t3*t124*1.173E-1+7.108330372102853E-16)+t5*(t142+t145+t146-t5*t7*3.4980534144E-1-t3*t59*1.173E-1+t3*t111)+1.725758919728622E-15);
  gra_compen[2] = t4*1.220247544E1+t102+t104+t154+t155+t156+t157+t158+t159+t160+t161-t5*t7*6.19394579584-t8*t31*6.781913737196921E-38-t8*t35*5.725223786013876E-18-t23*t27*6.781913737196921E-38-t23*t31*2.043497691865914E-37-t23*t38*5.725223786013876E-18-t3*t59*3.683E-1-t18*t45*1.023E-1+t5*t153*(2.51E2/6.25E2)+t6*(t147+t148-t5*t7*2.509184E1-t3*t59)*(2.51E2/6.25E2);
  gra_compen[3] = t102+t104+t154+t155+t156+t157+t158+t159+t160+t161-t5*t7*6.19394579584-t8*t31*6.781913737196921E-38-t8*t35*5.725223786013876E-18-t23*t27*6.781913737196921E-38-t23*t31*2.043497691865914E-37-t23*t38*5.725223786013876E-18-t3*t59*3.683E-1-t18*t45*1.023E-1;
  gra_compen[4] = t102+t104+t155+t156+t158+t159+t160+t161-t8*t31*6.781913737196921E-38-t8*t35*5.725223786013876E-18-t23*t27*6.781913737196921E-38-t23*t31*2.043497691865914E-37-t23*t38*5.725223786013876E-18-t18*t45*1.023E-1;
  gra_compen[5] = -t112-t113-t114-t115-t118+t135+t136+t137+t138+t139;
  gra_compen[6] = t77+t78+t79-t100-t101-t103;
}
