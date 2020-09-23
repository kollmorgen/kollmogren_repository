#include"koll_run.h"
#include"kollmorgen.h"
#include "canfestival.h"
#include "TestMaster.h"
# include"koll_unit_conver.h"
#include"koll_j1_pos.h"
#include"koll_tcp.h"
#include "koll_operation.h"

// ControlWord = SHUTDOWN;
// ControlWord = SWITCH_ON;
// ControlWord = ENABLE_OPERATION;

double TargetPosition_ph[7] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0};	
double ActualPosition_ph[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double TargetVelocity_ph[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double ActualVelocity_ph[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double TargetTorque_ph[7] = {0x0,0x0,0x0,0x0,0x0,0x0,};
double ActualTorque_ph[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};

double TargetPosition[7] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0};	
double TargetVelocity[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double TargetTorque[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double ActualPosition[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double ActualVelocity[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};
double ActualTorque[7] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0};



void* f_koll_run(void* args)
{
    printf("[run线程]:已经进入\n");
    while(1){
        int status=No_Command;
        pthread_mutex_lock(&mut_run);
        if (thread_run_flag != 0) {
            status=thread_run_flag;
            thread_run_flag=0;
        }
        pthread_mutex_unlock(&mut_run);
        switch (status){
        case No_Command     : break;
        case Command_stop      :  koll_stop(0);break;
        case Command_open_lock :  koll_open_the_lock();break;
        case Command_Shutdown  : return 0;
        // (2) 测试程序
        case Test_M1_q1  : koll_test_move( 1);break;
        case Test_M1_q2  : koll_test_move( 2);break;
        case Test_M1_q3  : koll_test_move( 3);break;
        case Test_M1_q4  : koll_test_move( 4);break;
        case Test_M1_q5  : koll_test_move( 5);break;
        case Test_M1_q6  : koll_test_move( 6);break;
        case Test_M1_qq1 : koll_test_move(-1);break;
        case Test_M1_qq2 : koll_test_move(-2);break;
        case Test_M1_qq3 : koll_test_move(-3);break;
        case Test_M1_qq4 : koll_test_move(-4);break;
        case Test_M1_qq5 : koll_test_move(-5);break;
        case Test_M1_qq6 : koll_test_move(-6);break;
        case Test_M1_s1  : koll_stop(1);break;
        case Test_M1_s2  : koll_stop(2);break;
        case Test_M1_s3  : koll_stop(3);break;
        case Test_M1_s4  : koll_stop(4);break;
        case Test_M1_s5  : koll_stop(5);break;
        case Test_M1_s6  : koll_stop(6);break;
        case Test_M2_q1_P: koll_test2_move_P(1);break;
        case Test_M2_q2_P: koll_test2_move_P(2);break;
        case Test_M2_q3_P: koll_test2_move_P(3);break;
        case Test_M2_q4_P: koll_test2_move_P(4);break;
        case Test_M2_q5_P: koll_test2_move_P(5);break;
        case Test_M2_q6_P: koll_test2_move_P(6);break;
        case Test_M2_q1_V: koll_test2_move_V(1);break;
        case Test_M2_q2_V: koll_test2_move_V(2);break;
        case Test_M2_q3_V: koll_test2_move_V(3);break;
        case Test_M2_q4_V: koll_test2_move_V(4);break;
        case Test_M2_q5_V: koll_test2_move_V(5);break;
        case Test_M2_q6_V: koll_test2_move_V(6);break;
        case debug_j1_para: j1_pos_move(debug_j1_para,0);break;
        // (3) operation　程序
        case Test_M3_run1: koll_operation_connect(Test_M3_run1);break;
        case Test_M3_run2: koll_operation_connect(Test_M3_run2);break;
        case Test_M3_run3: koll_operation_connect(Test_M3_run3);break;
        case Test_M3_run4: koll_operation_connect(Test_M3_run4);break;
        case Test_M4_no  : koll_operation_connect(Test_M4_no);break;
        case Test_M4_G   : koll_operation_connect(Test_M4_G);break;
        case Test_M4_GF  : koll_operation_connect(Test_M4_GF);break;
        case Test_M5_pos_init  : koll_operation_connect(Test_M5_pos_init);break;
        case Test_M5_pos_identif : koll_operation_connect(Test_M5_pos_identif);break;
        case Test_M5_mark1 : koll_test5_mark(1);break;
        case Test_M5_mark2 : koll_test5_mark(2);break;
        case Test_M5_mark3 : koll_test5_mark(3);break;
        case Test_M5_run_to_mark1 : koll_operation_connect(Test_M5_run_to_mark1);break;
        case Test_M5_run_to_mark2 : koll_operation_connect(Test_M5_run_to_mark2);break;
        case Test_M5_run_to_mark3 : koll_operation_connect(Test_M5_run_to_mark3);break;
        

        
        
        // case Test_M2_q1_10 : { koll_rel_turn_10(1);break;} break;
        // case Test_M2_q2_10 : { koll_rel_turn_10(2);break;} break;
        // case Test_M2_q3_10 : { koll_rel_turn_10(3);break;} break;
        // case Test_M2_q4_10 : { koll_rel_turn_10(4);break;} break;
        // case Test_M2_q5_10 : { koll_rel_turn_10(5);break;} break;
        // case Test_M2_q6_10 : { koll_rel_turn_10(6);break;} break;
        // case Test_M2_qq1_10 : { koll_rel_turn_10(-1);break;} break;
        // case Test_M2_qq2_10 : { koll_rel_turn_10(-2);break;} break;
        // case Test_M2_qq3_10 : { koll_rel_turn_10(-3);break;} break;
        // case Test_M2_qq4_10 : { koll_rel_turn_10(-4);break;} break;
        // case Test_M2_qq5_10 : { koll_rel_turn_10(-5);break;} break;
        // case Test_M2_qq6_10 : { koll_rel_turn_10(-6);break;} break;
        default:break;
        }
    }
    pthread_exit(NULL);
}


int koll_test_move(int joint){
    int ret1;
    if(ret1 = koll_operation_check() != 0){
        return -1;
    }
    printf("[run线程]:开始执行程序%d\n",joint);
    switch (joint){
    case  1: {j1_pos_check(); EnterMutex();OperationMode6 = 0x03;TargetVelocity_ph[1]=  1;koll_unit_conver(Physical_to_Nominal,1);TargetVelocity6=TargetVelocity[1];LeaveMutex();break;} break;
    case -1: {j1_pos_check(); EnterMutex();OperationMode6 = 0x03;TargetVelocity_ph[1]= -1;koll_unit_conver(Physical_to_Nominal,1);TargetVelocity6=TargetVelocity[1];LeaveMutex();break;} break;
    case  2: {EnterMutex();OperationMode5 = 0x03;TargetVelocity_ph[2]=  1;koll_unit_conver(Physical_to_Nominal,2);TargetVelocity5=TargetVelocity[2];LeaveMutex();break;} break;
    case -2: {EnterMutex();OperationMode5 = 0x03;TargetVelocity_ph[2]= -1;koll_unit_conver(Physical_to_Nominal,2);TargetVelocity5=TargetVelocity[2];LeaveMutex();break;} break;
    case  3: {EnterMutex();OperationMode4 = 0x03;TargetVelocity_ph[3]=  1;koll_unit_conver(Physical_to_Nominal,3);TargetVelocity4=TargetVelocity[3];LeaveMutex();break;} break;
    case -3: {EnterMutex();OperationMode4 = 0x03;TargetVelocity_ph[3]= -1;koll_unit_conver(Physical_to_Nominal,3);TargetVelocity4=TargetVelocity[3];LeaveMutex();break;} break;
    case  4: {EnterMutex();OperationMode3 = 0x03;TargetVelocity_ph[4]=  1;koll_unit_conver(Physical_to_Nominal,4);TargetVelocity3=TargetVelocity[4];LeaveMutex();break;} break;
    case -4: {EnterMutex();OperationMode3 = 0x03;TargetVelocity_ph[4]= -1;koll_unit_conver(Physical_to_Nominal,4);TargetVelocity3=TargetVelocity[4];LeaveMutex();break;} break;
    case  5: {EnterMutex();OperationMode2 = 0x03;TargetVelocity_ph[5]=  1;koll_unit_conver(Physical_to_Nominal,5);TargetVelocity2=TargetVelocity[5];LeaveMutex();break;} break;
    case -5: {EnterMutex();OperationMode2 = 0x03;TargetVelocity_ph[5]= -1;koll_unit_conver(Physical_to_Nominal,5);TargetVelocity2=TargetVelocity[5];LeaveMutex();break;} break;
    case  6: {EnterMutex();OperationMode1 = 0x03;TargetVelocity_ph[6]=  1;koll_unit_conver(Physical_to_Nominal,6);TargetVelocity1=TargetVelocity[6];LeaveMutex();break;} break;
    case -6: {EnterMutex();OperationMode1 = 0x03;TargetVelocity_ph[6]= -1;koll_unit_conver(Physical_to_Nominal,6);TargetVelocity1=TargetVelocity[6];LeaveMutex();break;} break;
    default: break;
    }
    return 0;
}


int koll_stop(int joint)
{
    printf("[run线程]:开始执行程序:全部停止\n");
    if (joint==0)
	{
		{
        koll_operation_connect(Command_stop);//终止目前的operation
		EnterMutex();
		switch(OperationMode6) {case 1: /* 关节1的位置模式出现了问题,不使用*/;break; case 3: TargetVelocity6=0; break;case 4:j1_pos_move(j1_stop,0) ;break;};
		switch(OperationMode5) {case 1: TargetPosition5= ActualPosition5;break; case 3: TargetVelocity5=0; break;case 4: ;break;};
		switch(OperationMode4) {case 1: TargetPosition4= ActualPosition4;break; case 3: TargetVelocity4=0; break;case 4: ;break;};
		switch(OperationMode3) {case 1: TargetPosition3= ActualPosition3;break; case 3: TargetVelocity3=0; break;case 4: ;break;};
		switch(OperationMode2) {case 1: TargetPosition2= ActualPosition2;break; case 3: TargetVelocity2=0; break;case 4: ;break;};
		switch(OperationMode1) {case 1: TargetPosition1= ActualPosition1;break; case 3: TargetVelocity1=0; break;case 4: ;break;};
		LeaveMutex();
		}
	}
	else
	{
		switch(joint)
		{
        EnterMutex();
		case 1: switch(OperationMode6) {case 1: /* 关节1的位置模式出现了问题,不使用*/;break;case 3: TargetVelocity6=0; break;case 4:j1_pos_move(j1_stop,0) ;break;};
		case 2: switch(OperationMode5) {case 1: TargetPosition5=ActualPosition5; break;case 3: TargetVelocity5=0; break;case 4: ;break;}break;
		case 3: switch(OperationMode4) {case 1: TargetPosition4=ActualPosition4; break;case 3: TargetVelocity4=0; break;case 4: ;break;}break;
		case 4: switch(OperationMode3) {case 1: TargetPosition3=ActualPosition3; break;case 3: TargetVelocity3=0; break;case 4: ;break;}break;
		case 5: switch(OperationMode2) {case 1: TargetPosition2=ActualPosition2; break;case 3: TargetVelocity2=0; break;case 4: ;break;}break;
		case 6: switch(OperationMode1) {case 1: TargetPosition1=ActualPosition1; break;case 3: TargetVelocity1=0; break;case 4: ;break;}break;
		LeaveMutex();
        
        }
	}
	return(0);
}
int koll_open_the_lock()
{
    printf("[run线程]:开始执行程序:开抱闸\n");
    EnterMutex();OperationMode6 = 0x03;LeaveMutex();
    EnterMutex();TargetVelocity6= 2000000;LeaveMutex();usleep(2000*1000);
    EnterMutex();TargetVelocity6=-2000000;LeaveMutex();usleep(2000*1000);
    EnterMutex();TargetVelocity6= 2000000;LeaveMutex();usleep(2000*1000);
    EnterMutex();TargetVelocity6=-2000000;LeaveMutex();usleep(2000*1000);
    EnterMutex();TargetVelocity6= 1000000;LeaveMutex();usleep(2000*1000);
    EnterMutex();TargetVelocity6= 0      ;LeaveMutex();usleep(100*1000);
    return 0;
}

int koll_rel_turn_10(int joint)
{
    printf("[run线程]:开始执行相对转动%d\n",joint);
    switch (joint){
    case  1: { j1_pos_move(relative_pos, 10);break;} break;
    case -1: { j1_pos_move(relative_pos,-10);break;} break;
    case  2: {EnterMutex();OperationMode5 = 0x01; ActualPosition[2]=ActualPosition5; TargetPosition5=ActualPosition[2]+14563; LeaveMutex();break;} break;
    case  3: {EnterMutex();OperationMode4 = 0x01; ActualPosition[3]=ActualPosition4; TargetPosition4=ActualPosition[3]+14563; LeaveMutex();break;} break;
    case  4: {EnterMutex();OperationMode3 = 0x01; ActualPosition[4]=ActualPosition3; TargetPosition3=ActualPosition[4]+14563; LeaveMutex();break;} break;
    case  5: {EnterMutex();OperationMode2 = 0x01; ActualPosition[5]=ActualPosition2; TargetPosition2=ActualPosition[5]+14563; LeaveMutex();break;} break;
    case  6: {EnterMutex();OperationMode1 = 0x01; ActualPosition[6]=ActualPosition1; TargetPosition1=ActualPosition[6]+145630; LeaveMutex();break;} break;
    case -2: {EnterMutex();OperationMode5 = 0x01; ActualPosition[2]=ActualPosition5; TargetPosition5=ActualPosition[2]-14563; LeaveMutex();break;} break;
    case -3: {EnterMutex();OperationMode4 = 0x01; ActualPosition[3]=ActualPosition4; TargetPosition4=ActualPosition[3]-14563; LeaveMutex();break;} break;
    case -4: {EnterMutex();OperationMode3 = 0x01; ActualPosition[4]=ActualPosition3; TargetPosition3=ActualPosition[4]-14563; LeaveMutex();break;} break;
    case -5: {EnterMutex();OperationMode2 = 0x01; ActualPosition[5]=ActualPosition2; TargetPosition2=ActualPosition[5]-14563; LeaveMutex();break;} break;
    case -6: {EnterMutex();OperationMode1 = 0x01; ActualPosition[6]=ActualPosition1; TargetPosition1=ActualPosition[6]-14563; LeaveMutex();break;} break;
    default: break;
    }
    return 0;
}


void j1_pos_move(int mode, double position)
{
    // EnterMutex();
    // printf("目前的状态：%d\n",OperationMode6);
    // LeaveMutex();
    if(mode == abso_pos){
        ji_pos_thread_connect();
        TargetPosition_ph[1] = position;
        pthread_mutex_lock(&mut_j1_pos);
        thread_j1pos_flag = j1_pos_change;
        pthread_mutex_unlock(&mut_j1_pos);
    }
    else if (mode == relative_pos){
        ji_pos_thread_connect();
        koll_unit_conver(Nominal_to_Physical,1);
        TargetPosition_ph[1] =  ActualPosition_ph[1] + position;
        pthread_mutex_lock(&mut_j1_pos);
        thread_j1pos_flag = j1_pos_change;
        pthread_mutex_unlock(&mut_j1_pos);
    }
    else if (mode == j1_stop){
        ji_pos_thread_connect();
        pthread_mutex_lock(&mut_j1_pos);
        thread_j1pos_flag = j1_pos_stop;
        pthread_mutex_unlock(&mut_j1_pos);
    }
    else if (mode == debug_j1_para){
        ji_pos_thread_connect();
        pthread_mutex_lock(&mut_j1_pos);
        thread_j1pos_flag = debug_j1_para;
        pthread_mutex_unlock(&mut_j1_pos);
    }
}


int koll_test2_move_P(int joint)
{
    int ret1;
    if(ret1 = koll_operation_check() != 0){
        return -1;
    }
    printf("[run线程]:关节%d开始Test2_P测试\n",joint);
    double num2=0.0;
    switch (joint){
    // case  1:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); j1_pos_move(abso_pos, num2); break;
    // case  2:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); EnterMutex();OperationMode5 = 0x01; TargetPosition5=(long)num2; LeaveMutex();break;
    // case  3:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); EnterMutex();OperationMode4 = 0x01; TargetPosition4=(long)num2; LeaveMutex();break;
    // case  4:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); EnterMutex();OperationMode3 = 0x01; TargetPosition3=(long)num2; LeaveMutex();break;
    // case  5:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); EnterMutex();OperationMode2 = 0x01; TargetPosition2=(long)num2; LeaveMutex();break;
    // case  6:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); EnterMutex();OperationMode1 = 0x01; TargetPosition1=(long)num2; LeaveMutex();break;


    case  1:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); j1_pos_move(abso_pos, num2); break;
    case  2:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetPosition_ph[2]=num2; koll_unit_conver(Physical_to_Nominal,2);EnterMutex();OperationMode5 = 0x01; TargetPosition5=TargetPosition[2]; LeaveMutex();break;
    case  3:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetPosition_ph[3]=num2; koll_unit_conver(Physical_to_Nominal,3);EnterMutex();OperationMode4 = 0x01; TargetPosition4=TargetPosition[3]; LeaveMutex();break;
    case  4:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetPosition_ph[4]=num2; koll_unit_conver(Physical_to_Nominal,4);EnterMutex();OperationMode3 = 0x01; TargetPosition3=TargetPosition[4]; LeaveMutex();break;
    case  5:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetPosition_ph[5]=num2; koll_unit_conver(Physical_to_Nominal,5);EnterMutex();OperationMode2 = 0x01; TargetPosition2=TargetPosition[5]; LeaveMutex();break;
    case  6:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetPosition_ph[6]=num2; koll_unit_conver(Physical_to_Nominal,6);EnterMutex();OperationMode1 = 0x01; TargetPosition1=TargetPosition[6]; LeaveMutex();break;
    default: break;
    }
    return 0;
} 

int koll_test2_move_V(int joint)
{
    printf("[run线程]:关节%d开始Test2_V测试\n",joint);
    double num2=0.0;
    switch (joint){
    case  1:  j1_pos_check(); //检查并关闭线程
              pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetVelocity_ph[1]=num2; koll_unit_conver(Physical_to_Nominal,1);EnterMutex();OperationMode6 = 0x03; TargetVelocity6=TargetVelocity[1]; LeaveMutex();break;
    case  2:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetVelocity_ph[2]=num2; koll_unit_conver(Physical_to_Nominal,2);EnterMutex();OperationMode5 = 0x03; TargetVelocity5=TargetVelocity[2]; LeaveMutex();break;
    case  3:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetVelocity_ph[3]=num2; koll_unit_conver(Physical_to_Nominal,3);EnterMutex();OperationMode4 = 0x03; TargetVelocity4=TargetVelocity[3]; LeaveMutex();break;
    case  4:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetVelocity_ph[4]=num2; koll_unit_conver(Physical_to_Nominal,4);EnterMutex();OperationMode3 = 0x03; TargetVelocity3=TargetVelocity[4]; LeaveMutex();break;
    case  5:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetVelocity_ph[5]=num2; koll_unit_conver(Physical_to_Nominal,5);EnterMutex();OperationMode2 = 0x03; TargetVelocity2=TargetVelocity[5]; LeaveMutex();break;
    case  6:  pthread_mutex_lock(&mut_tcp); num2 = test2_num; pthread_mutex_unlock(&mut_tcp); TargetVelocity_ph[6]=num2; koll_unit_conver(Physical_to_Nominal,6);EnterMutex();OperationMode1 = 0x03; TargetVelocity1=TargetVelocity[6]; LeaveMutex();break;
    default: break;
    }
    return 0;
}



// 3.2-test5-记录位置
void koll_test5_mark(int command)
{
    int k=0;
    koll_unit_conver(Nominal_to_Physical ,0);
    if(command == 1){
        for (k=1;k<=6;k++){
            mark1_postion[k]=ActualPosition_ph[k];
        }
    }
    else if(command == 2){
        for (k=1;k<=6;k++){
            mark2_postion[k]=ActualPosition_ph[k];
        }
    }
    else if(command == 3){
        for (k=1;k<=6;k++){
            mark3_postion[k]=ActualPosition_ph[k];
        }
    }
}