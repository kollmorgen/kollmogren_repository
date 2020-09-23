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

#include "Master.h"
#include "rgm6.h"
#include "rgmtcp.h"
// #include "RGMikControl.hpp"
#include "math.h"

int CANopen_status = 0;
int matsaa=0;
struct _pid
{
  float target;             //定义设定值  
  float actual;             //定义实际值  
  float err;                //定义偏差值   
  float err_last;           //定义上一个偏差值  
  float Kp,Ki,Kd;           //定义比例、积分、微分系数  
  float operation;            //定义执行值  
  float integral;           //定义积分值
} pid1,pid2,pid3,pid4,pid5,pid6;

extern s_BOARD MasterBoard;
int isinit = 0;
int count = 0;
int timecount = 0;
int step_pvt = 0;
const char end_request[] = "PVT_FINISHED\n";

long long time_now = 0,time_last=0,time_use=0;
struct timeval tv;
/*****************************************************************************/
void TestMaster_heartbeatError(CO_Data* d, UNS8 heartbeatID)
{
	eprintf("TestMaster_heartbeatError %d\n", heartbeatID);
}

/*****************************************************************************/
INTEGER8 Operation_Mode = PROFILE_VELOCITY_MODE;
void TestMaster_initialisation(CO_Data* d)
{
	UNS32 RPDO_COBID[7] = {0x80000282, 0x80000292, 0x800002A2, 0x80000212, 0x80000222, 0x80000232, 0x80000242}; 
	UNS32 TPDO_COBID[7] = {0x80000402, 0x80000412, 0x80000422, 0x80000302, 0x80000452, 0x80000462, 0x80000472};
	UNS32 size = sizeof(UNS32); 
	// UNS32 SINC_cicle=0;
	// UNS8 data_type = 0;	
	eprintf("TestMaster_initialisation\n");

	// change tpdo state based on opearation_mode
	switch(Operation_Mode)
	{
		case PROFILE_POSITION_MODE :
			TPDO_COBID[0] &= ~0x80000000; //[控制字+目标位置]
			TPDO_COBID[3] &= ~0x80000000; //[状态字+操作状态]
			break;
		case PROFILE_VELOCITY_MODE :
			TPDO_COBID[0] &= ~0x80000000; //[控制字+目标位置]
			TPDO_COBID[2] &= ~0x80000000;
			TPDO_COBID[1] &= ~0x80000000; //[控制字+目标速度]
			TPDO_COBID[3] &= ~0x80000000; //[状态字+操作状态]
			// TPDO_COBID[4] &= ~0x80000000;
			break;
		case PROFILE_TORQUE_MODE :
			TPDO_COBID[2] &= ~0x80000000;
			TPDO_COBID[3] &= ~0x80000000;
			break;
		case CYCLIC_POSITION_MODE :
			TPDO_COBID[4] &= ~0x80000000;
			TPDO_COBID[3] &= ~0x80000000;
			break;
		case CYCLIC_VELOCITY_MODE :
			TPDO_COBID[5] &= ~0x80000000;  //[目标速度]
			TPDO_COBID[3] &= ~0x80000000;  //[状态字+操作状态]
			break;
		case CYCLIC_TORQUE_MODE :
			TPDO_COBID[6] &= ~0x80000000;
			TPDO_COBID[3] &= ~0x80000000;
			break;
		default :
			break;
	}
	for (int i = 0; i < 7; i++)  //7个组
	{
		for (int j = 0; j < 6; j++) //6各节点
		{
			UNS32 TPDO_COBId = TPDO_COBID[i] + j;			
			if(0x1800 + i*6+j == 0x1806){
				printf("\nTPDO_COBId = %x\n",TPDO_COBId);
			}
			//设置本地对象字典
			writeLocalDict( &TestMaster_Data, /*CO_Data* d*/
			0x1800 + i*6+j, /*UNS16 index索引*/
			0x01, /*UNS8 subind子索引*/ 
			&TPDO_COBId, /*void * pSourceData,*/ 
			&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
			
		}
	}
}

//brief:Step counts number of times 
//       ConfigureSlaveNode is called
//      index means node id
static int init_step[6] = {0, 0, 0, 0, 0, 0};
//brief: the index of TPDO/RPDO 	
static int init_index[6] = {0, 0, 0, 0, 0, 0};
static void ConfigureSlaveNode(CO_Data* d, UNS8 nodeId);		/*Froward declaration*/

static void CheckSDOAndContinue(CO_Data* d, UNS8 nodeId)
{
	UNS32 abortCode;	
	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
		eprintf("!!!!!!!! Master : Failed in initializing slave %2.2x, step %d, index %d AbortCode :%4.4x \n", nodeId, init_step[nodeId-2], init_index[nodeId-2], abortCode);
	/* Finalise last SDO transfer with this node */
	closeSDOtransfer(&TestMaster_Data, nodeId, SDO_CLIENT);
	ConfigureSlaveNode(d, nodeId);
}

/********************************************************
 * ConfigureSlaveNode is responsible to
 *  - setup slave TPDO and RPDO transmit type 设置传输类型
 *  - switch to operational mode  切换操作模式
 *  - send NMT to slave 发送nmt的信号
 *  - 这里是用作从站的设定
 ********************************************************/ 
static void ConfigureSlaveNode(CO_Data* d, UNS8 nodeId)
{
	
	//brief: PDO_COBID index
	//cobid
	UNS32 TPDO_COBID[10] = {0x80000282, 0x80000292, 0x800002A2, 0x80000212, 0x80000222, 0x80000232, 0x80000242, 0x800002C2, 0x800002D2, 0x800002E2}; 
	UNS32 RPDO_COBID[10] = {0x80000402, 0x80000412, 0x80000422, 0x80000302, 0x80000452, 0x80000462, 0x80000472, 0x800004C2, 0x800004D2, 0x800004E2}; 
	// UNS16 TPDO_INDEX[10] = {0x1800, 0x1802, 0x1803, 0x1804, 0x1814, 0x1815, 0x1816, 0x1817, 0x1818, 0x1819};
	// UNS16 RPDO_INDEX[10] = {0x1400, 0x1401, 0x1402, 0x1403, 0x1404, 0x1414, 0x1415, 0x1416, 0x1417, 0x1419};
	UNS16 TPDO_INDEX[10] = {0x1802, 0x1803, 0x1804, 0x1800, 0x1814, 0x1815, 0x1816, 0x1817, 0x1818, 0x1819};
	UNS16 RPDO_INDEX[10] = {0x1402, 0x1403, 0x1404, 0x1401, 0x1414, 0x1415, 0x1416, 0x1400, 0x1417, 0x1419};
	UNS8 res;
	
	//brief : select operation mode
	switch(Operation_Mode)
	{
		case PROFILE_POSITION_MODE :  
			RPDO_COBID[0] &= ~0x80000000;
			RPDO_COBID[1] &= ~0x80000000;
			RPDO_COBID[2] &= ~0x80000000;
			RPDO_COBID[3] &= ~0x80000000;
			RPDO_COBID[4] &= ~0x80000000;
			RPDO_COBID[5] &= ~0x80000000;
			break;
		case PROFILE_VELOCITY_MODE :
			RPDO_COBID[0] &= ~0x80000000;
			RPDO_COBID[2] &= ~0x80000000;
			RPDO_COBID[1] &= ~0x80000000;
			RPDO_COBID[3] &= ~0x80000000;
			break;
		case PROFILE_TORQUE_MODE :
			RPDO_COBID[2] &= ~0x80000000;
			RPDO_COBID[3] &= ~0x80000000;
			break;
		case CYCLIC_POSITION_MODE :
			RPDO_COBID[4] &= ~0x80000000;
			RPDO_COBID[3] &= ~0x80000000;
			break;
		case CYCLIC_VELOCITY_MODE :
			RPDO_COBID[5] &= ~0x80000000;
			RPDO_COBID[3] &= ~0x80000000;
			break;
		case CYCLIC_TORQUE_MODE :
			RPDO_COBID[6] &= ~0x80000000;
			RPDO_COBID[3] &= ~0x80000000;
			break;
		default :
			break;
	}
	
	//brief: plus one after index dict go next
	if (init_step[nodeId-2] == 7)
	{
		init_index[nodeId-2]++;
		if (init_index[nodeId-2] < 10)
		{
			init_step[nodeId-2] = 0;
		}		
	}
	//真正最后的cobid
	UNS32 TPDO_COBid;
	UNS32 RPDO_COBid;
	UNS16 TPDO_Index;
	UNS16 RPDO_Index;
	// brief : change PDO COB-ID or index with init_index 
	if (!((init_step[nodeId-2] == 9)&&(init_index[nodeId-2] == 10)))
	{
		TPDO_COBid = TPDO_COBID[init_index[nodeId-2]] + nodeId - 0x02;
		RPDO_COBid = RPDO_COBID[init_index[nodeId-2]] + nodeId - 0x02;
		TPDO_Index = TPDO_INDEX[init_index[nodeId-2]];
		RPDO_Index = RPDO_INDEX[init_index[nodeId-2]];
 
		// eprintf("nodeId : %2.2x; init_step: %d; init_index: %d; TPDO_Index: %4.4x; RPDO_Index: %4.4x, TPDO_COBid: %8.8x, RPDO_Index: %8.8x\n", nodeId,
	 	// 			init_step[nodeId-2],
		// 			init_index[nodeId-2],
		// 			TPDO_Index,
		// 			RPDO_Index,
		// 			TPDO_COBid,
		// 			RPDO_COBid
		// 			);
	}

	switch(++init_step[nodeId-2])
	{
	case 1:
	{
		//TPDO_COBid |= 0x80000000;
		eprintf("Master : slave %2.2x TPDO %4.4x TPDO_COBid %x\n", nodeId, TPDO_Index, TPDO_COBid); //节点号-
		res = writeNetworkDictCallBack (d, /*CO_Data* d*/ //SDO发送配置子站
				nodeId, /*UNS8 nodeId*/
				TPDO_Index, /*UNS16 index*/
				0x01, /*UNS8 subindex*/
				4, /*UNS8 count*/
				0, /*UNS8 dataType*/
				&TPDO_COBid,/*void *data*/
				CheckSDOAndContinue, /*SDOCallback_t Callback*/
            	0); /* use block mode */
	}
	break;
	case 2:
	{
		UNS8 Transmission_Type = 0x01;
		eprintf("Master : set slave %2.2x TPDO %4.4x transmit type %x\n", nodeId, TPDO_Index,Transmission_Type);
		res = writeNetworkDictCallBack (d, /*CO_Data* d*/
				nodeId, /*UNS8 nodeinit_stepId*/
				TPDO_Index, /*UNS16 index*/
				0x02, /*UNS8 subindex*/
				1, /*UNS8 count*/
				0, /*UNS8 dataType*/
				&Transmission_Type,/*void *data*/
				CheckSDOAndContinue, /*SDOCallback_t Callback*/
            	0); /* use block mode */
	}
	break;
	case 3: 
	{
		if (init_index[nodeId-2] <= 4)
		{
		TPDO_COBid &= ~0x80000000;
		}
		eprintf("Master : re-enable or re-disable slave %2.2x TPDO %4.4x TPDO_COBid %x\n", nodeId, TPDO_Index,TPDO_COBid);
		res = writeNetworkDictCallBack (d, /*CO_Data* d*/
				nodeId, /*UNS8 nodeId*/
				TPDO_Index, /*UNS16 index*/
				0x01, /*UNS8 subindex*/
				4, /*UNS8 count*/
				0, /*UNS8 dataType*/
				&TPDO_COBid,/*void *data*/
				CheckSDOAndContinue, /*SDOCallback_t Callback*/
                0); /* use block mode */
	}
	break;
	case 4:
	{
		RPDO_COBid |= 0x80000000;
		eprintf("Master : disable slave %2.2x RPDO %4.4x RPDO_COBid %x\n", nodeId, RPDO_Index,RPDO_COBid); 
		res = writeNetworkDictCallBack (d, /*CO_Data* d*/
				nodeId, /*UNS8 nodeId*/
				RPDO_Index, /*UNS16 index*/
				0x01, /*UNS8 subindex*/
				4, /*UNS8 count*/
				0, /*UNS8 dataType*/
				&RPDO_COBid,/*vo#include "Master.h"id *data*/
				CheckSDOAndContinue, /*SDOCallback_t Callback*/
            	0); /* use block mode */
	}
	break;
	case 5:
	{
		UNS8 Transmission_Type = 0x01;
		eprintf("Master : set slave %2.2x RPDO %4.4x receive  %d\n", nodeId, RPDO_Index,Transmission_Type);
		res = writeNetworkDictCallBack (d, /*CO_Data* d*/
				nodeId, /*UNS8 nodeId*/
				RPDO_Index, /*UNS16 index*/
				0x02, /*UNS8 subindex*/
				1, /*UNS8 count*/
				0, /*UNS8 dataType*/
				&Transmission_Type,/*void *data*/
				CheckSDOAndContinue, /*SDOCallback_t Callback*/
            	0); /* use block mode */
	}
	break;
	case 6: 
	{
		// RPDO_COBid &= ~0x80000000;
		eprintf("Master : re-enable or re-disable slave %2.2x RPDO %4.4x RPDO_COBid %x\n", nodeId, RPDO_Index,RPDO_COBid);
		res = writeNetworkDictCallBack (d, /*CO_Data* d*/
				nodeId, /*UNS8 nodeId*/
				RPDO_Index, /*UNS16 index*/
				0x01, /*UNS8 subindex*/
				4, /*UNS8 count*/
				0, /*UNS8 dataType*/
				&RPDO_COBid,/*void *data*/
				CheckSDOAndContinue, /*SDOCallback_t Callback*/
                0); /* use block mode */
	}			
	break;
	case 7:
	{
		UNS16 Heartbeat_Producer_Time = 0x0000; 
		eprintf("Master : set slave %2.2x heartbeat producer time \n", nodeId);
		res = writeNetworkDictCallBack (d, /*CO_Data* d*/
				nodeId, /*UNS8 nodeId*/
				0x1017, /*UNS16 index*/
				0x00, /*UNS8 subindex*init_index*/
				2, /*UNS8 count*/
				0, /*UNS8 dataType*/
				&Heartbeat_Producer_Time,/*void *data*/
				CheckSDOAndContinue, /*SDOCallback_t Callback*/
                0); /* use block mode */
	}
	break;

	break;
	case 8: /* START */
		if (nodeId == 0x07)
		{
			/* Put the master in operational mode  run out 6 RGM upset process*/
			setState(d, Operational);
			//  /* Ask slave node to go in operational mode */
			masterSendNMTstateChange (d, 0x00, NMT_Start_Node);
		}
		else
		{
			masterSendNMTstateChange(&TestMaster_Data, nodeId+0x01, NMT_Reset_Node);
			//printf("test nodeId + 0x01 = %d \n ",nodeId+0x01);
		}
		
		break;	
	}
}


void TestMaster_preOperational(CO_Data* d)
{
	eprintf("TestMaster_preOperational\n");
	masterSendNMTstateChange(&TestMaster_Data, 0x02, NMT_Reset_Node);
	eprintf("TestMaster_preOperational 222\n");
}

void TestMaster_operational(CO_Data* d) //1006h是窗口时间
{
	eprintf("TestMaster_operational\n");
	// UNS32 Cycle_Period = 0x0000C350;
	UNS32 Cycle_Period = 0x0001356;
	UNS32 size = sizeof(UNS32); 
	writeLocalDict( &TestMaster_Data, /*CO_Data* d*/
			0x1006, /*UNS16 index*/
			0x00, /*UNS8 subind*/ 
			&Cycle_Period, /*void * pSourceData,*/ 
			&size, /* UNS8 * pExpectedSize*/
			RW);  /* UNS8 checkAccess */
	eprintf("TestMaster_Operational 222\n");
}

void TestMaster_stopped(CO_Data* d)
{
	eprintf("TestMaster_stopped\n");
}


int rec_output = 0;
int display = 0;
int rec_state_change = 0;
int pidh=0;
void TestMaster_post_sync(CO_Data* d)
{
	count++;

	int position_sum = 0;
	// if ((display == 1)&&(count < 30))
	// {
	// 	//eprintf("Position3 : %8.8ld  Velocity3 : %8.8ld  ActualTorque3 : %8.8ld\n", ActualPosition3, ActualVelocity3, ActualTorque3);
	// }
	// eprintf("Position3 : %8.8ld  Velocity3 : %8.8ld  ActualTorque3 : %8.8ld\n", ActualPosition3, ActualVelocity3, ActualTorque3);
	// eprintf("AP6 : %d    AP5 : %d    AP4 : %d    AP3 : %d    AP2 : %d    AP1 : %d\n" , ActualPostion6,ActualPostion5,ActualPostion4,ActualPostion3,ActualPostion2,ActualPostion1);
	
	// OperationMode6 = 0x03; //01位置模式 03速度模式 04电流模式
	// TargetVelocity6 = -20000;
	// OperationMode6 = 0x03; //01位置模式 03速度模式 04电流模式
	// TargetVelocity6 = 200000;
	// printf("Actual4:%d  ;%d  :%d  Actual6:%d  ;%d  :%d\n",ActualPosition4,ActualVelocity4,ActualTorque4,ActualPosition6,ActualVelocity6,ActualTorque6);
	// if (pos_control_invel==1)
	// {
	// 	if(pidh==0)
	// 	{
			
	// 		PID_init();
			
	// 		// printf("请输入[Kp Kd Ki]参数:\n");
	// 		// scanf("%f %f %f",&pid1.Kp,&pid1.Kd,&pid1.Ki);
	// 		// printf("Kp=%f,Kd=%f,Ki=%f\n",pid1.Kp,pid1.Kd,pid1.Ki);

	// 		pidh=1;
	// 		pidcount=0;
	// 	}
	// 	switch (pos_control_invel)
	// 	{
	// 	    case 1:pos_control_invel_move(1);break;
	// 	}
	// }

    
	
	switch(isinit){
		case 0 :
			if (
				 (ActualPosition1<0x100000)&&(ActualPosition1>0x0)&&
				 (ActualPosition2<0x100000)&&(ActualPosition2>0x0)&&
				(ActualPosition3<0x100000)&&(ActualPosition3>0x0)&&
				 (ActualPosition4<0x100000)&&(ActualPosition4>0x0)&&
				 (ActualPosition5<0x100000)&&(ActualPosition5>0x0)&&
				 //(ActualPosition6<0x100000)&&(ActualPosition6>0x0)&&
				(1)
			)
			{
				// EnterMutex();
				TargetPosition1 = ActualPosition1;
				TargetPosition2 = ActualPosition2;
				TargetPosition3 = ActualPosition3;
				TargetPosition4 = ActualPosition4;
				TargetPosition5 = ActualPosition5;
				TargetPosition6 = ActualPosition6;
				ControlWord = SHUTDOWN;
				isinit = 1;
				// LeaveMutex();
			printf("Master :INITIALIZATION STEP 1 Controlword = SHUTDOWN\n");
			}
			else
			printf("ERROR:WRONG WORK AREA!!!!\n");
			break;		
		case 1 :
			// EnterMutex();
			ControlWord = SWITCH_ON;
			isinit = 2;
			printf("Master :INITIALIZATION STEP 2 Controlword = SWITCH_ON\n");
			// LeaveMutex();
			break;
		case 2 :
			// EnterMutex();
			ControlWord = ENABLE_OPERATION;
			isinit = 3;
			// LeaveMutex();
			printf("Master :INITIALIZATION STEP 3 Controlword = ENABLE_OPERATION\n");
			printf("[CANopen线程]:CANopen初始化完成\n");
			CANopen_status=1;
			break;
		default :
			break;
		}

	// // REPORT TO PC
	
	// 	canopen_queue();
	// 	if(count == 100){
	// 		count = 0;
	// 		//printf("TargetPosition1,ActualTar = %x,%x\n",TargetPosition1,ActualPosition1);
		
	// 		gettimeofday(&tv,NULL);
	// 		time_last = time_now;
	// 		time_now = 1000000*tv.tv_sec+tv.tv_usec;
	// 		time_use=time_now-time_last;
	// 		//printf("time_use = %lld\n",time_use);
	// 		}
   			

	// //manage pvt_command
	// if(control_mode == MOTION_PLAN){
	// 	Enter_pvtqueue_Mutex();
	// 	// rec_state_change = get_pos_wrap(handle,ActualPosition1,ActualPosition2,
	// 	// 								ActualPosition3,ActualPosition4,
	// 	// 								ActualPosition5,ActualPosition6,rec_output);
		
	// 	// /*calculate velocity*/
	// 	// rec_output = calcuVelocity_wrap(handle,&TargetVelocity1,&TargetVelocity2,
	// 	// 									&TargetVelocity3,&TargetVelocity4,
	// 	// 									&TargetVelocity5,&TargetVelocity6);
	// 	// // switch(rec_output){
	// 	// 	case 0 :
	// 	// 		break;
	// 	// 	case 1 :

	// 	// 		/*read actual position to calculate fedposition*/
	// 	// 		break;
	// 	// 	case -1 :
	// 	// 		printf("PVT_QUEUE_FINISHED!!\n");

	// 	// 		position_sum = ActualPosition1 - position_sum;

	// 	// 		printf("position_sum = %x\n", position_sum);

	// 	//         /*pop out traj control method*/
	// 	// 		trajCtrlDele_wrap(&handle);


	// 	// 		/*send end request*/
	// 	// 		if((rec_output=send(sock_fd,end_request,strlen(end_request),MSG_NOSIGNAL)) < 0){
    //     //                         printf("ERROR:Fail to send string\n");
    //     //                         close(sock_fd);
    //     //                         exit(1);
    //     //                         tcp_connected = 0;
    //     //                         return 0;
    //     //                     }
	// 	// 		rec_output = 0;
	// 	// 		rec_state_change = 0;
				
	// 	// 		/*destroy pvt command message queue from phy*/
	// 	// 		destory_pvt_queue(&pvt_command_queue);
	// 	// 		control_mode = COMMAND;
	// 	// 		break;			
	// 	// 	}
	// 	Leave_pvtqueue_Mutex();
		
	// 	}

	// 	if (control_mode == FEEDBACK_CONTROL)
	// 	{
	// 		Enter_pvtqueue_Mutex();

	// 		rec_state_change = get_pos_ik_wrap(handle,ActualPosition1,ActualPosition2,
	// 											ActualPosition3,ActualPosition4,
	// 											ActualPosition5,ActualPosition6,pRGM->target_tcp_frame);

	// 		rec_output = calcuVelocity_wrap_ctrl(handle,&TargetVelocity1,&TargetVelocity2,&TargetVelocity3,
	// 									&TargetVelocity4,&TargetVelocity5,&TargetVelocity6);


	// 		rec_state_change = 0;

	// 		rec_output = 0;

	// 		Leave_pvtqueue_Mutex();
	// 	}
//   matsaa++;
//   if(matsaa==250){
// 	  printf("hello i am here :8.16\n");
// 	  matsaa=0;
//   }

	
}

void TestMaster_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg)
{
	//eprintf("Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\n", nodeID, errCode, errReg);
}

char query_result = 0;
char waiting_answer = 0;

static void CheckSDO(CO_Data* d, UNS8 nodeId)
{
	UNS32 abortCode;	
	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
		eprintf("Master : Failed in changing Slave's transmit type AbortCode :%4.4x \n", abortCode);
	/* Finalise last SDO transfer with this node */
	closeSDOtransfer(&TestMaster_Data, nodeId, SDO_CLIENT);
}

static int MasterSyncCount = 0;
void TestMaster_post_TPDO(CO_Data* d)
{
}

void TestMaster_post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
	printf("---------------------------\n");
	eprintf("TestMaster_post_SlaveBootup %x\n", nodeid);
	eprintf("Master : ConfigureSlaveNode %2.2x\n", nodeid);
	ConfigureSlaveNode(d, nodeid);
}


void pos_control_invel_move(int joint)
{
	
	kollmorgen_unit_conversion(0);//驱动转化为物理
	//double k=fabs(TargetPosition[1]-ActualPosition6);
	//double t=(TargetPosition[1]-ActualPosition6);
	//printf("1号电机状态:%d,k=%lf,t=%lf,vel=%d\n",OperationMode6,k,t,ActualVelocity6);
    //printf("111-t=%lf,记录:%d,目标位置:%lf,实际位置:%d\n",t,group0[1],TargetPosition[1],ActualPosition6);
    pidcount++;
    pid1.actual=ActualPosition6;
	pid1.target=TargetPosition[1]; 
    pid1.err=pid1.target-pid1.actual; 
    pid1.integral+=pid1.err;  
    pid1.operation=pid1.Kp*pid1.err+pid1.Ki*pid1.integral+pid1.Kd*(pid1.err-pid1.err_last);   
    float kkk=pid1.err-pid1.err_last;
	pid1.err_last=pid1.err;
	
	if(fabs(pid1.err)<=100)
	{
		pid1.err=0;
	
	} 
	if(pid1.operation>=3000) pid1.operation=3000; 
	if(pid1.operation<=-3000) pid1.operation=-3000; 
    TargetTorque6= pid1.operation;
	if(pidcount==100)
	{
		 printf("位置偏差:%f  速度偏差:%f  输出数据:%f\n",pid1.err,kkk,pid1.operation);
		pidcount=0;
		timecount++;

	}
	if(timecount==10)
	{
		pid1.err=0;
		pos_control_invel=0;
		pidh=0;
		timecount=0;
		TargetVelocity6=0;
	} 
	// if(k<=200.0)
	// {  
	// 	//printf("\n");
	// 	/*!!!!!!!!这个地方是不可以上锁的？？？？为什么*/
	// 	//EnterMutex();
	// 	pos_control_invel=0;
    //     TargetVelocity6=0;
	// 	//LeaveMutex();
	// 	printf("运动结束\n");
	// }
	// else if(t>=50000)
	// {
	// 	TargetVelocity_ph[1]= 10;kollmorgen_unit_conversion(3);kollmorgen_move(1);
	// } 
	// else if(t<=-50000)
	// {
	// 	TargetVelocity_ph[1]= -10;kollmorgen_unit_conversion(3);kollmorgen_move(1);

	// } 
	// else 
	// {
	// 	if (t>0)
	// 	{
	// 		TargetVelocity_ph[1]= t*t*0.0002*0.00002;kollmorgen_unit_conversion(3);kollmorgen_move(1);
	// 	}
    //     else
	// 	{
	// 		TargetVelocity_ph[1]= t*t*0.0002*0.00002;kollmorgen_unit_conversion(3);kollmorgen_move(1);
	// 	}
	// 	// TargetVelocity_ph[1]= t*0.0002;kollmorgen_unit_conversion(3);kollmorgen_move(1);

		  
	// }

}


void PID_init()
{
  //printf("PID_init begin \n");  
  /*初始化设置*/ 
  pid1.target=0.0; pid1.actual=0.0; pid1.err=0.0; pid1.err_last=0.0; pid1.operation=0.0; pid1.integral=0.0; 
  pid2.target=0.0; pid2.actual=0.0; pid2.err=0.0; pid2.err_last=0.0; pid2.operation=0.0; pid2.integral=0.0; 
  pid3.target=0.0; pid3.actual=0.0; pid3.err=0.0; pid3.err_last=0.0; pid3.operation=0.0; pid3.integral=0.0; 
  pid4.target=0.0; pid4.actual=0.0; pid4.err=0.0; pid4.err_last=0.0; pid4.operation=0.0; pid4.integral=0.0;  
  pid6.target=0.0; pid6.actual=0.0; pid6.err=0.0; pid6.err_last=0.0; pid6.operation=0.0; pid6.integral=0.0; 
  /*PID参数*/
  pid1.Kp=1;  pid1.Ki=0.0; pid1.Kd=20;//200 0 50000
  pid2.Kp=0.2;  pid2.Ki=0.015; pid2.Kd=0.2;
  pid3.Kp=0.2;  pid3.Ki=0.015; pid3.Kd=0.2;
  pid4.Kp=0.2;  pid4.Ki=0.015; pid4.Kd=0.2;
  pid5.Kp=0.2;  pid5.Ki=0.015; pid5.Kd=0.2;
  pid6.Kp=0.2;  pid6.Ki=0.015; pid6.Kd=0.2;
  //printf("PID_init end \n");
}
/*******************************************
*            关节1-PID位置控制             *
*      输入：理想位置  输出：执行位置      * 
*******************************************/
float PID1_realize(float pos)
{

}