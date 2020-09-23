#include "koll_operation.h"
#include "koll_unit_conver.h"
#include "TestMaster.h"
#include "canfestival.h"
#include "koll_run.h"
# include "koll_dynamics.h"
#include"koll_j1_pos.h"

//此处是关于补偿函数
double model_compen[7]={0,0,0,0,0,0,0};


struct itimerval timenow;
int opeation_status = operation_finished;
int operation_command = operation_no_command;
int display_count;
int operation=0;


FILE *fp,*fp_d;            /*文件指针*/
//(1)激励轨迹参数

float TRAJ[300000][19] ;  
float TRAJ_d[300000][7]; 

int ap[7]={0,0,0,262144,0,262144,0};//位置-初始偏差(输入单位)
int ep[7]={0,37033,8723,-4371,13109,2,-16019}; //位置-调整偏差(输入单位)
float p_ph[7];

double operation_postion[7]={0,45,90,0,90,0,0};
double init_postion[7]={0,45,90,0,90,0,0};
double identifi_postion[7]={0,0,90,90,90,0,0};
double mark1_postion[7]={0,45,90,0,90,0,0};
double mark2_postion[7]={0,45,90,0,90,0,0};
double mark3_postion[7]={0,45,90,0,90,0,0};

struct _exci_tra_para
{
  long long count;
  long long count2;
  float t_end;          //定义单次总执行时间 
  float t_now;          //定义现在时间 
  float time;          //定义总执行次数
  int in_p[7];
  double in_pd[7];
  double in_pdd[7];
} tra;

// 0.1-检查线程是否正在运行的程序
int koll_operation_check() //非operation，非stop程序执行时候的检查程序
{
    pthread_mutex_lock(&mut_operation);
    int status = opeation_status; 
    pthread_mutex_unlock(&mut_operation);
    if ( status == operation_not_finish){/*尚未停止运行*/
        printf("Not finished! Please waite or stop operation\n");
        return -1;
    }
    else{
        return 0;
    }
}

// 0.2-开启线程+线程是否已经开启检查
int koll_operation_connect(int run_command) //开启线程+线程是否已经开启检查
{
    pthread_mutex_lock(&mut_operation);
    int status = opeation_status; 
    pthread_mutex_unlock(&mut_operation);

    if ( status == operation_not_finish && run_command == Command_stop ){/*紧急停止*/
        operation_command = operation_stop;
        if (operation==operation_current){
        EnterMutex();
        OperationMode1=3; TargetVelocity1=0;
        OperationMode2=3; TargetVelocity2=0;
        OperationMode3=3; TargetVelocity3=0;
        OperationMode4=3; TargetVelocity4=0;
        OperationMode5=3; TargetVelocity5=0;
        OperationMode6=3; TargetVelocity6=0;
        LeaveMutex();
        }
    }

    else if ( status == operation_not_finish){/*尚未停止运行*/
        printf("Not finished! Please waite or stop operation\n");
    }

    else if ( status == operation_finished && run_command != Command_stop){/*可以运行新的一组命令*/
        operation_command = run_command;
        pthread_mutex_lock(&mut_operation);
        opeation_status = operation_not_finish;
        pthread_mutex_unlock(&mut_operation);
        int th_operation = pthread_create(&pt_operation,NULL, f_koll_operation, NULL);
        if (th_operation != 0){
	    	printf("ERROR:fail to start operation thread\n");
	        return -1;
	    }
        else{
            printf("[operation线程]:已经进入\n");
        }
        
    }
    return 0;
}

// 0.3-定时器开启函数
void start_timer()/*默认定时器是1ms*/
{
 	timenow.it_interval.tv_sec = 0;
	timenow.it_interval.tv_usec = 10*1000;
	timenow.it_value.tv_sec = 0;
	timenow.it_value.tv_usec = 1*1000;
	setitimer(ITIMER_REAL, &timenow, NULL);
}
// 0.4-定时器关闭函数
void close_timer()/*默认定时器是1ms*/
{
 	timenow.it_interval.tv_sec = 0;
	timenow.it_interval.tv_usec = 0*1000;
	timenow.it_value.tv_sec = 0;
	timenow.it_value.tv_usec = 0*1000;
	setitimer(ITIMER_REAL, &timenow, NULL);
}
// 0.5-线程主函数
void* f_koll_operation(void* args)/*函数解析，并开启相应的中断函数*/
{
    operation=0;
    display_count=0;

    int k=0;
    
    //TRAJ是实际数据采集；TRAJ_d是理想数据---清空数组
    memset(TRAJ, 0, sizeof(TRAJ)); memset(TRAJ_d, 0, sizeof(TRAJ_d));

    switch(operation_command){
        //***指令**********末端时间********执行次数*****开始计数******连接中断信号*************************设置状态属性***打开文本文件，并且清空**********************关闭文本************************************************************开启定时器*************
        case Test_M3_run1: tra.t_end=10; tra.time=1; tra.count=0; signal(SIGALRM, koll_test_M3_run1);operation=1;if((fp=fopen("tra1.txt","w"))==NULL);    fclose(fp); if((fp_d=fopen("tra1_d.txt","w"))==NULL);fclose(fp_d); start_timer(); break;
        case Test_M3_run2: tra.t_end=10; tra.time=1; tra.count=0; signal(SIGALRM, koll_test_M3_run2);operation=2;if((fp=fopen("tra2.txt","w"))==NULL);    fclose(fp); if((fp_d=fopen("tra2_d.txt","w"))==NULL);fclose(fp_d); start_timer();koll_test_M3_run22(); break;
        case Test_M3_run3: tra.t_end=10; tra.time=1; tra.count=0; signal(SIGALRM, koll_test_M3_run3);operation=3;if((fp=fopen("tra3.txt","w"))==NULL);    fclose(fp); if((fp_d=fopen("tra3_d.txt","w"))==NULL);fclose(fp_d); start_timer(); break;
        case Test_M3_run4: tra.t_end=20; tra.time=1; tra.count=0; signal(SIGALRM, koll_test_M3_run4);operation=4;if((fp=fopen("tra4.txt","w"))==NULL);    fclose(fp); if((fp_d=fopen("tra4_d.txt","w"))==NULL);fclose(fp_d); start_timer(); break;
        
        case Test_M4_no: operation=operation_current; koll_No_com();break;
        case Test_M4_G:  operation=operation_current; koll_Gra_com();break;
        case Test_M4_GF: operation=operation_current; koll_Gra_Ff_com();break;

        case Test_M5_mark1: koll_test5_mark(1); break;
        case Test_M5_mark2: koll_test5_mark(2); break;
        case Test_M5_mark3: koll_test5_mark(3); break;
        case Test_M5_pos_init:     for(k=1;k<=6;k++) operation_postion[k] = init_postion[k];     koll_test5_move();break;
        case Test_M5_pos_identif:  for(k=1;k<=6;k++) operation_postion[k] = identifi_postion[k]; koll_test5_move();break;
        case Test_M5_run_to_mark1: for(k=1;k<=6;k++) operation_postion[k] = mark1_postion[k];    koll_test5_move();break;
        case Test_M5_run_to_mark2: for(k=1;k<=6;k++) operation_postion[k] = mark2_postion[k];    koll_test5_move();break;
        case Test_M5_run_to_mark3: for(k=1;k<=6;k++) operation_postion[k] = mark3_postion[k];    koll_test5_move();break;
        default: break;   
    }

    
    while(1){
        if (operation_command == operation_stop) {//如果此时正是电流控制模式，那么我们就切换到速度模式，默认速度为0

            break;
        }
        pthread_mutex_lock(&mut_operation);
        int status = opeation_status;
        pthread_mutex_unlock(&mut_operation);
        if (status == operation_finished){
            break;
        } 
    }
    close_timer();
    operation_command = operation_no_command;
    pthread_mutex_lock(&mut_operation);
    opeation_status = operation_finished;
    pthread_mutex_unlock(&mut_operation);
    //记录数据
    switch (operation){
        case 1 : write_status(1); break;
        case 2 : write_status(2); break;
        case 3 : write_status(3); break;
        case 4 : write_status(4); break;
        default: break;
    }
    printf("[operation线程]:已经退出\n");
    pthread_exit(NULL);
}


// 1.1-test3-激励轨迹1运行函数 
void koll_test_M3_run1()
{
    tra.count++;
    tra.t_now =  tra.count*0.01;
    if (tra.t_now >= tra.t_end * tra.time){
        note_end();
        usleep(10*1000);
        pthread_mutex_lock(&mut_operation);
        opeation_status = operation_finished;
        pthread_mutex_unlock(&mut_operation);
    }
    float t_use = fmod( tra.t_now , tra.t_end);
    operation_fourier_tra1(t_use);//计算目标位置 
    koll_test3_move_P();
    note_status(tra.t_now);
     display_count++;
     if(display_count==100){
         display_count=0;
        printf("time:%f\n",tra.t_now);
     }

}

                                                                            

// 1.2-test3-激励轨迹2运行函数
void koll_test_M3_run2()
{

    tra.count++;
    tra.t_now =  tra.count*0.01;
    if (tra.t_now >= tra.t_end * tra.time){
        note_end();
        usleep(10*1000);
        pthread_mutex_lock(&mut_operation);
        opeation_status = operation_finished;
        pthread_mutex_unlock(&mut_operation);
        printf("此处时间已到\n");    
    }
    float t_use = fmod( tra.t_now , tra.t_end);
    //operation_fourier_tra2(t_use);//计算目标位置 
    //koll_test3_move_P();
    //note_status(tra.t_now);
     display_count++;
     if(display_count==100){
         display_count=0;
        printf("time1:%f\n",tra.t_now);
     }
}
void koll_test_M3_run22()
{
    float t_now;
    int here_count=0;
    tra.count2=0;
    int mydisplay=1000;
    while(1){
        if (operation_command == operation_stop) {
            break;
        }
        pthread_mutex_lock(&mut_operation);
        int status = opeation_status;
        pthread_mutex_unlock(&mut_operation);
        if (status == operation_finished){
            break;
        } 
        tra.count2++;
        t_now =  tra.count2*0.001;
        if (t_now >= tra.t_end * tra.time){
            note_end();
            usleep(10*1000);
            pthread_mutex_lock(&mut_operation);
            opeation_status = operation_finished;
            pthread_mutex_unlock(&mut_operation);
            printf("此处时间已到2\n");
            break;
        }   
        float t_use = fmod( t_now , tra.t_end);
        operation_fourier_tra2(t_use);//计算目标位置 
        koll_test3_move_P();
        note_status(t_now);
        here_count++;
        if(here_count==1000){
             here_count=0;
            printf("time2:%f\n",tra.t_now);
        }
        //if (tra.count>=100)
        usleep(910);
    }
}
// 1.3-test3-激励轨迹3运行函数

void koll_test_M3_run3()
{
    tra.count++;
    tra.t_now =  tra.count*0.01;
    if (tra.t_now >= tra.t_end * tra.time){
        note_end();
        usleep(10*1000);
        pthread_mutex_lock(&mut_operation);
        opeation_status = operation_finished;
        pthread_mutex_unlock(&mut_operation);
    }
    float t_use = fmod( tra.t_now , tra.t_end);
    operation_fourier_tra3(t_use);//计算目标位置 
    koll_test3_move_P();
    note_status(tra.t_now);
     display_count++;
     if(display_count==100){
         display_count=0;
        printf("time:%f\n",tra.t_now);
     }
}

// 1.4-test3-激励轨迹4运行函数
void koll_test_M3_run4()
{
    tra.count++;
    tra.t_now =  tra.count*0.01;
    if (tra.t_now >= tra.t_end * tra.time){
        note_end();
        usleep(10*1000);
        pthread_mutex_lock(&mut_operation);
        opeation_status = operation_finished;
        pthread_mutex_unlock(&mut_operation);
    }
    float t_use = fmod( tra.t_now , tra.t_end);
    operation_fourier_tra4(t_use);//计算目标位置 
    koll_test3_move_P();
    note_status(tra.t_now);
     display_count++;
     if(display_count==100){
         display_count=0;
        printf("time:%f\n",tra.t_now);
     }
}

// 1.5-test3-激励轨迹最终执行运行函数
int koll_test3_move_P()
{
    //j1_pos_move(abso_pos, tra.in_p[1]); break;
    //TargetPosition_ph[2]=tra.in_p[2]; koll_unit_conver(Physical_to_Nominal,2);EnterMutex();OperationMode5 = 0x01; TargetPosition5=TargetPosition[2]; LeaveMutex();
    //TargetPosition_ph[3]=tra.in_p[3]; koll_unit_conver(Physical_to_Nominal,3);EnterMutex();OperationMode4 = 0x01; TargetPosition4=TargetPosition[3]; LeaveMutex();
    //TargetPosition_ph[4]=tra.in_p[4]; koll_unit_conver(Physical_to_Nominal,4);EnterMutex();OperationMode3 = 0x01; TargetPosition3=TargetPosition[4]; LeaveMutex();
    j1_pos_move(abso_pos, (double)p_ph[1]);
    //EnterMutex(); OperationMode5 = 0x01; TargetPosition5=tra.in_p[2]; LeaveMutex();
    //EnterMutex(); OperationMode4 = 0x01; TargetPosition4=tra.in_p[3]; LeaveMutex();
    //EnterMutex(); OperationMode3 = 0x01; TargetPosition3=tra.in_p[4]; LeaveMutex();
    //EnterMutex(); OperationMode2 = 0x01; TargetPosition2=tra.in_p[5]; LeaveMutex();
    //EnterMutex(); OperationMode1 = 0x01; TargetPosition1=tra.in_p[6]; LeaveMutex();

    return 0;
} 

// 1.6-test3-激励轨迹记录数据函数
void note_status(float t)
{
        //输出轨迹的实际曲线
        koll_unit_conver(Nominal_to_Physical,0);
        TRAJ[tra.count2][0]  = t;
        TRAJ[tra.count2][1]  = ActualPosition_ph[1];
        TRAJ[tra.count2][2]  = ActualPosition_ph[2];
        TRAJ[tra.count2][3]  = ActualPosition_ph[3];
        TRAJ[tra.count2][4]  = ActualPosition_ph[4];
        TRAJ[tra.count2][5]  = ActualPosition_ph[5];  
        TRAJ[tra.count2][6]  = ActualPosition_ph[6];
        TRAJ[tra.count2][7]  = ActualVelocity_ph[1];
        TRAJ[tra.count2][8]  = ActualVelocity_ph[2];
        TRAJ[tra.count2][9]  = ActualVelocity_ph[3];
        TRAJ[tra.count2][10] = ActualVelocity_ph[4];
        TRAJ[tra.count2][11] = ActualVelocity_ph[5];  
        TRAJ[tra.count2][12] = ActualVelocity_ph[6];
        TRAJ[tra.count2][13] = ActualTorque_ph[1];
        TRAJ[tra.count2][14] = ActualTorque_ph[2];
        TRAJ[tra.count2][15] = ActualTorque_ph[3];
        TRAJ[tra.count2][16] = ActualTorque_ph[4];
        TRAJ[tra.count2][17] = ActualTorque_ph[5];  
        TRAJ[tra.count2][18] = ActualTorque_ph[6];
        //输出轨迹的目标轨迹曲线
        TRAJ_d[tra.count2][0] = t;
        TRAJ_d[tra.count2][1] = p_ph[1];
        TRAJ_d[tra.count2][2] = p_ph[2];
        TRAJ_d[tra.count2][3] = p_ph[3];
        TRAJ_d[tra.count2][4] = p_ph[4];
        TRAJ_d[tra.count2][5] = p_ph[5];  
        TRAJ_d[tra.count2][6] = p_ph[6];
}

// 1.7-test3-激励轨迹记录终止位置函数
void note_end()
{
    //输出轨迹2的实际曲线
    TRAJ[tra.count2][0] = 9999;
    TRAJ[tra.count2+1][0] = 9999;
    printf("时间位置：%ld\n",tra.count);
    //输出轨迹2的目标轨迹曲线
    TRAJ_d[tra.count2][0] = 9999;
    TRAJ_d[tra.count2+1][0] = 9999;
}

// 1.8-test3-激励轨迹生成轨迹txt文件函数
void write_status(int traj)
{
    switch (traj){
        case 1:  fp=fopen("tra1.txt","ab"); fp_d=fopen("tra1_d.txt","ab");break;
        case 2:  fp=fopen("tra2.txt","ab"); fp_d=fopen("tra2_d.txt","ab");break;
        case 3:  fp=fopen("tra3.txt","ab"); fp_d=fopen("tra3_d.txt","ab");break;
        case 4:  fp=fopen("tra4.txt","ab"); fp_d=fopen("tra4_d.txt","ab");break;
        default: break;
    }
    //输出实际数值
    int i=1;
    while(TRAJ[i][0]<=1000){
        if(TRAJ[i][0]>0){
            fprintf(fp,"%8.2f,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf\n"\
                                                                          ,TRAJ[i][0],\
                                                                           TRAJ[i][1],\
                                                                           TRAJ[i][2],\
                                                                           TRAJ[i][3],\
                                                                           TRAJ[i][4],\
                                                                           TRAJ[i][5],\
                                                                           TRAJ[i][6],\
                                                                           TRAJ[i][7],\
                                                                           TRAJ[i][8],\
                                                                           TRAJ[i][9],\
                                                                           TRAJ[i][10],\
                                                                           TRAJ[i][11],\
                                                                           TRAJ[i][12],\
                                                                           TRAJ[i][13],\
                                                                           TRAJ[i][14],\
                                                                           TRAJ[i][15],\
                                                                           TRAJ[i][16],\
                                                                           TRAJ[i][17],\
                                                                           TRAJ[i][18]);
        }
        i++;
    }
        
    //输出目标轨迹曲线数值
    i=1;
    while(TRAJ_d[i][0]<=1000){
        if(TRAJ_d[i][0]>0){
            fprintf(fp_d,"%8.2f,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf,%8.2lf\n",TRAJ_d[i][0],\
                                                                             TRAJ_d[i][1],\
                                                                             TRAJ_d[i][2],\
                                                                             TRAJ_d[i][3],\
                                                                             TRAJ_d[i][4],\
                                                                             TRAJ_d[i][5],\
                                                                             TRAJ_d[i][6]);
        }
        i++;
    }

    fclose(fp);
    fclose(fp_d);
}

// 1.9-test3-激励轨迹1生成函数
void operation_fourier_tra1(double t)
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

  
  p_ph[1] = t*(-1.269284191636659E1)-t8*1.415144842310716E1-t9*3.319071129765705E-1+t*t8*4.099573876258091+t*t9*7.615705149819954E-3+t2*t10*2.097898408684882-t2*t11*2.566714966259575E1+t2*t12*2.589509109816197E1-t2*t13*6.635368887084005-t2*t14*1.849185323801058E1+t2*t15*1.454746534809246E1-t2*t16*6.616460156282042E1+t2*t17*1.603709744838878E1+t2*t18*1.860042790419576E1-t2*t19*7.74599855129344+7.257905398648393;
  p_ph[2] = t*(-2.834393751471628)-t8*1.006783474100025E1-t9*1.43194253682077E-1+t*t8*2.297006323347214+t*t9*1.700636250882977E-3+t2*t10*3.590234013770829+t2*t11*3.803726338651687E1-t2*t12*1.437699410457977E1-t2*t13*4.157741352195223-t2*t14*1.20687519793101E1-t2*t15*1.089518031494607E1+t2*t16*5.186704704485416E1-t2*t17*2.488212831732892E1+t2*t18*1.106136439605156E1+t2*t19*8.340246683434369+8.649094864300569E1;
  p_ph[3] = t*2.955409196245879E1-t8*1.50566033965757E1+t9*2.927453454711249E-1+t*t8*5.59114830692613E-2-t*t9*1.773245517747528E-2-t2*t10*1.061533886701647E1+t2*t11*6.917456863351296E1-t2*t12*7.239825699783546-t2*t13*1.184944105317019E1+t2*t14*5.76124310285252-t2*t15*9.297098975196164-t2*t16*7.310549981446426E1+t2*t17*1.314080592351767E1-t2*t18*3.002339347645444-t2*t19*1.136351473148697E1+7.560245992913464E1;
  p_ph[4] = t*2.391740206683298E1-t8*8.056777670665952+t9*2.781932542958352E-1-t*t8*7.803846725501076E-1-t*t9*1.435044124009979E-2-t2*t10*3.387058404074434-t2*t11*3.94931898316659E1+t2*t12*2.271914023997128E1-t2*t13*6.279134836366662-t2*t14*2.401393541684231-t2*t15*1.358094456035473E1+t2*t16*6.893693208691923E1-t2*t17*3.688821059896746E1-t2*t18*4.964262273165542-t2*t19*7.987502900469734+9.91805779915049E1;
  p_ph[5] = t*1.301151874488344E1+t8*1.496596150340772E1+t9*3.448323962073289E-1-t*t8*4.294344175169889-t*t9*7.806911246930065E-3+t2*t10*9.138818145099208+t2*t11*3.667771411760958E1-t2*t12*1.633911677175625E1-t2*t13*1.562491055924178E1+t2*t14*1.118894809463415E1-t2*t15*1.163030614553412E1+t2*t16*2.435342038413799E1-t2*t17*1.507381750813793E1+t2*t18*3.587835857474045-t2*t19*2.968838984257718-7.97094206269259;
  p_ph[6] = t*3.287248408876359E1-t8*2.042182622085385E1+t9*2.888689991229154E-1+t*t8*7.971168352944115E-1-t*t9*1.972349045325816E-2-t2*t10*3.925736474522484+t2*t11*1.907004711598127E1-t2*t12*3.320849852458219E1-t2*t13*9.350073692570907-t2*t14*1.810339807669332-t2*t15*9.147841576091849-t2*t16*3.43735302196679E1-t2*t17*3.566440332904707E1+t2*t18*1.254743580770411-t2*t19*4.171276606976984+9.302479540105132;
 
  //tra.in_p[1]= (int)(p_ph[1]* (65536/45))+ap[1]-ep[1];
  tra.in_p[1]=  p_ph[1];
  tra.in_p[2]= (int)(p_ph[2]* (65536/45))+ap[2]-ep[2];
  tra.in_p[3]=-(int)(p_ph[3]* (65536/45))+ap[3]-ep[3];
  tra.in_p[4]= (int)(p_ph[4]* (65536/45))+ap[4]-ep[4];
  tra.in_p[5]= (int)(p_ph[5]* (65536/45))+ap[5]-ep[5];
  tra.in_p[6]= (int)(p_ph[6]* (65536/45))+ap[6]-ep[6];
 }





// 1.10-test3-激励轨迹2生成函数
void operation_fourier_tra2(double t)
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


  p_ph[1] = t*8.560321612789311+t3*9.293874387283543E-1-t4*1.041909649024602+t5*1.376986985791232E-1-t6*5.136192967673586E-3+t7*t9*1.36490491228523E-1+t8*t9*2.461791979595574+t9*t14*1.21864684385627E+1+t9*t15*7.263872141352401E-1-t9*t16*7.880760301458517+t9*t17*4.200888976838955-t9*t18*3.765982172731643E+1-t9*t19*6.317316724090594E-1-t9*t20*1.455438230219261-t9*t21*2.955242049783022-2.982396463335473;
  p_ph[2] = t*(-1.037901522304992E+1)+t3*4.203451122506658+t4*1.972112978036604E-1-t5*1.136507171206823E-1+t6*6.227409133829953E-3+t7*t9*3.744575559933239E-1-t8*t9*4.687375762720009+t9*t14*3.581136613727847E+1+t9*t15*1.115590145062947E+1-t9*t16*4.852245881943293+t9*t17*1.298362782316955+t9*t18*1.936604377802885E+1+t9*t19*1.698950196352191E+1-t9*t20*3.34609267294077E-1+t9*t21*5.747683756414801+7.606189698265305E+1;
  p_ph[3] = t*3.941079192049486-t3*5.325460345947926+t4*6.709841499846368E-1+t5*5.861584421263032E-3-t6*2.364647515229692E-3-t7*t9*7.995472390566217+t8*t9*1.113007651609898-t9*t14*2.227938461805021E+1-t9*t15*7.315097867532776+t9*t16*6.801505432975774+t9*t17*6.590981445362878+t9*t18*1.356484258514189E+1-t9*t19*3.67634868753056+t9*t20*5.784279112969104-t9*t21*1.220885419182125E+1+9.770229328431899E+1;
  p_ph[4] = t*5.151204955239516+t3*4.186859546502989-t4*1.352492404824549+t5*1.191366697936226E-1-t6*3.09072297314371E-3+t7*t9*8.812248493954169+t8*t9*1.855395458896727+t9*t14*2.825568147263894E+1+t9*t15*5.250846542690653-t9*t16*2.537835685685511-t9*t17*1.125555122095887E+1-t9*t18*1.736117990875693E+1-t9*t19*7.707612193480806-t9*t20*8.866590275691499+t9*t21*6.085793263027957+8.092008648223557E+1;
  p_ph[5] = t*2.80161048025669+t3*3.212804527488838-t4*9.227219535234362E-1+t5*7.41522024787387E-2-t6*1.680966288154014E-3+t7*t9*5.711307766612492E-1+t8*t9*1.16030956653184+t9*t14*2.648467271306405E+1-t9*t15*2.735141595584123-t9*t16*3.599283570202016+t9*t17*3.356534284896645-t9*t18*6.231090192443141+t9*t19*1.720275774475741-t9*t20*3.125336779573373-t9*t21*1.910762812932718-7.664237622061784;
  p_ph[6] = t*(-9.44120089553952)+t3*8.951811271271517E-1+t4*7.650838641285217E-1-t5*1.326662021618213E-1+t6*5.664720537323712E-3-t7*t9*1.838389373336291E-2-t8*t9*2.979203338183956-t9*t14*1.913761765292223+t9*t15*3.728166483534245E-3-t9*t16*2.079801435806283+t9*t17*2.207744343714673+t9*t18*3.814914663013388E+1+t9*t19*1.127202193259606E+1-t9*t20*4.223811072522565E-1+t9*t21*6.689934987620361E-1+5.731088601115491E-1;
  
  //tra.in_p[1]= (int)(p_ph[1]* (65536/45))+ap[1]-ep[1];
  tra.in_p[1]=  p_ph[1];
  tra.in_p[2]= (int)(p_ph[2]* (65536/45))+ap[2]-ep[2];
  tra.in_p[3]=-(int)(p_ph[3]* (65536/45))+ap[3]-ep[3];
  tra.in_p[4]= (int)(p_ph[4]* (65536/45))+ap[4]-ep[4];
  tra.in_p[5]= (int)(p_ph[5]* (65536/45))+ap[5]-ep[5];
  tra.in_p[6]= (int)(p_ph[6]* (65536/45))+ap[6]-ep[6];

  
 
 }

// 1.11-test3-激励轨迹3生成函数
 void operation_fourier_tra3(double t)
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
  p_ph[1] = t*3.413150768014772+t3*5.32786752249123-t4*1.406888581299723+t5*1.044759367451339E-1-t6*2.047890460808864E-3+t7*t9*3.455511492714479-t8*t9*1.130134514889162+t9*t14*4.811516729275876E+1-t9*t15*2.58357023429177+t9*t16*4.568256234565286-t9*t17*5.030458409388519-t9*t18*1.153771049478816E+1-t9*t19*7.175536188890219-t9*t20*6.943304428155243E-1+t9*t21*4.13917323384678-1.544595742573769E+1;
  p_ph[2] = t*(-2.219748534300089)+t3*1.246157757780389-t4*2.725669812606892E-2-t5*2.083465043669744E-2+t6*1.331849120580053E-3-t7*t9*2.075545709010996-t8*t9*3.736970847707179+t9*t14*1.803507822704177E+1-t9*t15*1.502508527565549-t9*t16*5.263672665027768+t9*t17*6.691866831003213+t9*t18*3.919217601575307-t9*t19*2.086075457797265+t9*t20*3.055005255701696E-1+t9*t21*7.280007161836264+8.494357801661869E+1;
  p_ph[3] = t*(-7.056968139019869)-t3*9.270447182577108+t4*2.559786250417408-t5*1.985589939110691E-1+t6*4.234180883411922E-3+t7*t9*2.905832166127304+t8*t9*2.558084531549056-t9*t14*3.73962926272505E+1+t9*t15*6.569110633380175-t9*t16*1.04526587962506E+1-t9*t17*7.187235891946595+t9*t18*1.554374021089798E+1+t9*t19*1.322598877418446E+1-t9*t20*1.008573819733058E+1+t9*t21*2.68897871751973+1.045025945562608E+2;
  p_ph[4] = t*1.166183551705395E+1+t3*9.234968556016-t4*3.013177262908595+t5*2.672772183159693E-1-t6*6.997101310232373E-3-t7*t9*1.612500250806139+t8*t9*6.672207308008673E-1+t9*t14*2.935834435623246E+1-t9*t15*1.031820253894722E+1+t9*t16*9.263042379220251+t9*t17*7.239917509721538-t9*t18*1.636136789642065E+1-t9*t19*2.464338426898177E+1+t9*t20*1.057656449081633E+1-t9*t21*6.931709569334723+7.919955411257742E+1;
  p_ph[5] = t*(-7.27717442895332)+t3*5.334726526018159-t4*3.392278623082993E-1-t5*5.58103511741182E-2+t6*4.366304657371993E-3+t7*t9*3.519261125671957-t8*t9*6.032645277628181+t9*t14*4.743634840083173E+1-t9*t15*1.22178424214694E+1+t9*t16*3.446736219866002-t9*t17*2.041395198101414+t9*t18*4.487730951700581E+1-t9*t19*1.493943346147892+t9*t20*3.81693179475224+t9*t21*3.302220080985192-1.277794817890495E+1;
  p_ph[6] = t*1.064078106871648E+1+t3*4.103054534278705-t4*1.884689013727389+t5*2.006422613735343E-1-t6*6.38446864122989E-3-t7*t9*7.702323570463122-t8*t9*1.050139843797545+t9*t14*3.494807386128718E+1+t9*t15*8.704708832245589+t9*t16*4.193776699246269+t9*t17*9.396833179293698-t9*t18*4.870649926835751E+1+t9*t19*1.83237243238596-t9*t20*7.067579532537881+t9*t21*4.572821719151134-1.576941203532568E+1;
 
  tra.in_p[1]=  p_ph[1];
  tra.in_p[2]= (int)(p_ph[2]* (65536/45))+ap[2]-ep[2];
  tra.in_p[3]=-(int)(p_ph[3]* (65536/45))+ap[3]-ep[3];
  tra.in_p[4]= (int)(p_ph[4]* (65536/45))+ap[4]-ep[4];
  tra.in_p[5]= (int)(p_ph[5]* (65536/45))+ap[5]-ep[5];
  tra.in_p[6]= (int)(p_ph[6]* (65536/45))+ap[6]-ep[6];
 }


// 1.12-test3-激励轨迹4生成函数
void operation_fourier_tra4(double t)
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
  p_ph[1] = t*(-3.224931412063841)-t2*1.868518541246288+t3*2.674751394262247E-1-t4*1.071804275073542E-2+t5*1.20934927952394E-4+t6*t12*6.693065784261684+t6*t13*7.06929707000894E-1-t6*t14*1.091487357313827E+1+t6*t15*4.450838148065941E-1+t6*t16*1.372980510190052E+1-t6*t17*2.05939093137836-t6*t18*6.407314223884445E+1-t6*t19*5.599647651641183+t6*t20*1.757074153715141E+1-t6*t21*2.25629765627786+2.329635826233939E+1;
  p_ph[2] = t*(-4.98710891479843)+t2*1.92616911539645-t3*6.793918866968425E-2-t4*4.535406426755933E-3+t5*1.870165843049413E-4-t6*t12*1.532398346127331E+1+t6*t13*2.868568504883934E+1+t6*t14*2.153419109611018E+1-t6*t15*1.203502012271384E+1+t6*t16*2.766394867719378E+1-t6*t17*8.294222583155135+t6*t18*4.235468344713762E+1+t6*t19*4.532044144544874E-1+t6*t20*5.666724697964172E+1+t6*t21*1.040931192004826E+1+6.52661184585854E+1;
  p_ph[3] = t*6.177211514579021-t2*2.838166930236752+t3*1.293864051591997E-1+t4*4.486854264243781E-3-t5*2.316454317967133E-4+t6*t12*5.01534503747691-t6*t13*2.826003706406154E+1+t6*t14*3.641879035482938+t6*t15*6.59117375465177-t6*t16*2.287379023479015E+1+t6*t17*1.635357679042727E+1-t6*t18*6.186968658316211E+1-t6*t19*2.104746379689926E+1-t6*t20*7.530760781356456E+1-t6*t21*1.302903426587109E+1+1.226331178722413E+2;
  p_ph[4] = t*6.968914747486057-t2*7.898499024856598E-1-t3*9.523787843858546E-2+t4*1.10920903953222E-2-t5*2.613343030307271E-4+t6*t12*5.965339537992506-t6*t13*1.084384361564073E+1-t6*t14*2.162412513312796E+1+t6*t15*4.344060769035011-t6*t16*7.450277253380353-t6*t17*8.476992097723985-t6*t18*6.122073053813239E+1+t6*t19*2.790724579649249E+1-t6*t20*7.840108829469012E+1+t6*t21*1.193338662410372E+1+1.090400604241502E+2;
  p_ph[5] = t*5.944306455714509-t2*1.618541970564746+t3*1.324653566361189E-2+t4*7.099219678052838E-3-t5*2.229114920892942E-4+t6*t12*6.813535001110127-t6*t13*2.298107598968429E+1-t6*t14*4.811110824059934+t6*t15*1.437949157457616E-1+t6*t16*9.678122703568706+t6*t17*8.889122331497967-t6*t18*6.638822349140348E+1-t6*t19*4.231961831053794-t6*t20*8.878107914038093E+1-t6*t21*8.764564909540749+2.915681542303851E+1;
  p_ph[6] = t*(-9.888491058369965)-t2*1.58536124806925+t3*4.057484012661741E-1-t4*2.250432385461681E-2+t5*3.708184146888737E-4-t6*t12*5.200036093452777+t6*t13*8.287897731961733+t6*t14*6.46067036245606+t6*t15*2.63027388606605+t6*t16*1.09200000762982E+1-t6*t17*7.682425799785664E-1-t6*t18*4.896142624132867E+1-t6*t19*6.498579981790987+t6*t20*7.732242524830482E+1-t6*t21*3.451971309205852+1.461407613418409E+1;
 
  tra.in_p[1]=  p_ph[1];
  tra.in_p[2]= (int)(p_ph[2]* (65536/45))+ap[2]-ep[2];
  tra.in_p[3]=-(int)(p_ph[3]* (65536/45))+ap[3]-ep[3];
  tra.in_p[4]= (int)(p_ph[4]* (65536/45))+ap[4]-ep[4];
  tra.in_p[5]= (int)(p_ph[5]* (65536/45))+ap[5]-ep[5];
  tra.in_p[6]= (int)(p_ph[6]* (65536/45))+ap[6]-ep[6];
 
 }

// 2.1-test4-全部设置为电流模式

void koll_Gra_Ff_com()
{
    j1_pos_check();

    EnterMutex();
	OperationMode1 = 0x04;
	OperationMode2 = 0x04;
	OperationMode3 = 0x04;
	OperationMode4 = 0x04;
	OperationMode5 = 0x04;
	OperationMode6 = 0x04;
	LeaveMutex();

    while(1){
        if (operation_command == operation_stop) {
            break;
        }
        pthread_mutex_lock(&mut_operation);
        int status = opeation_status;
        pthread_mutex_unlock(&mut_operation);
        if (status == operation_finished){
            break;
        } 


        koll_unit_conver(Nominal_to_Physical,0);

        double p[7];
		p[1]=ActualPosition_ph[1];
		p[2]=ActualPosition_ph[2];
		p[3]=ActualPosition_ph[3];
		p[4]=ActualPosition_ph[4];
		p[5]=ActualPosition_ph[5];
		p[6]=ActualPosition_ph[6];
		
        koll_gravity_compensation(p);  
	    
		EnterMutex();
        double v[7];
        v[1]=ActualVelocity6;
		v[2]=ActualVelocity5;
		v[3]=ActualVelocity4;
		v[4]=ActualVelocity3;
		v[5]=ActualVelocity2;
		v[6]=ActualVelocity1;
		LeaveMutex();

        double Ff_com[7];
        Ff_com[1]=0;
		Ff_com[2]=0;
		Ff_com[3]=0;
		Ff_com[4]=0;
		Ff_com[5]=0;
		Ff_com[6]=0;
		if(v[1]>1000) Ff_com[1]=  8.00;  else if(v[1]<-1000) Ff_com[1]= -8.00; else Ff_com[1]=0;			
		if(v[2]>100)  Ff_com[2]=  3.50;  else if(v[2]<-100)  Ff_com[2]= -3.50; else Ff_com[2]=0;			
        if(v[3]>100)  Ff_com[3]= -12.00;  else if(v[3]<-100)  Ff_com[3]=  8.00; else Ff_com[3]=0;			
		if(v[4]>500)  Ff_com[4]=  4.00;  else if(v[4]<-500)  Ff_com[4]= -4.00; else Ff_com[4]=0;			
		if(v[5]>100)  Ff_com[5]=  4.00;  else if(v[5]<-100)  Ff_com[5]= -4.00; else Ff_com[5]=0;			
		if(v[6]>100)  Ff_com[6]=  5.00;  else if(v[6]<-100)  Ff_com[6]= -5.00; else Ff_com[6]=0;			
	
		TargetTorque_ph[1]= model_compen[1]+Ff_com[1];koll_unit_conver(Physical_to_Nominal,1);koll_test4_move();
		TargetTorque_ph[2]= model_compen[2]+Ff_com[2];koll_unit_conver(Physical_to_Nominal,2);koll_test4_move();
		TargetTorque_ph[3]= model_compen[3]+Ff_com[3];koll_unit_conver(Physical_to_Nominal,3);koll_test4_move();
		TargetTorque_ph[4]= model_compen[4]+Ff_com[4];koll_unit_conver(Physical_to_Nominal,4);koll_test4_move();
		TargetTorque_ph[5]= model_compen[5]+Ff_com[5];koll_unit_conver(Physical_to_Nominal,5);koll_test4_move();
		TargetTorque_ph[6]= model_compen[6]+Ff_com[6];koll_unit_conver(Physical_to_Nominal,6);koll_test4_move();
	}
}

void koll_Gra_com()
{
    j1_pos_check();

    EnterMutex();
	OperationMode1 = 0x04;
	OperationMode2 = 0x04;
	OperationMode3 = 0x04;
	OperationMode4 = 0x04;
	OperationMode5 = 0x04;
	OperationMode6 = 0x04;
	LeaveMutex();

    while(1){
        if (operation_command == operation_stop) {
            break;
        }
        pthread_mutex_lock(&mut_operation);
        int status = opeation_status;
        pthread_mutex_unlock(&mut_operation);
        if (status == operation_finished){
            break;
        } 


        koll_unit_conver(Nominal_to_Physical,0);

        double p[7];
		p[1]=ActualPosition_ph[1];
		p[2]=ActualPosition_ph[2];
		p[3]=ActualPosition_ph[3];
		p[4]=ActualPosition_ph[4];
		p[5]=ActualPosition_ph[5];
		p[6]=ActualPosition_ph[6];
		
        koll_gravity_compensation(p);  

        double Ff_com[7];
        Ff_com[1]=0;
		Ff_com[2]=0;
		Ff_com[3]=0;
		Ff_com[4]=0;
		Ff_com[5]=0;
		Ff_com[6]=0;			
	
		TargetTorque_ph[1]= model_compen[1]+Ff_com[1];koll_unit_conver(Physical_to_Nominal,1);koll_test4_move();
		TargetTorque_ph[2]= model_compen[2]+Ff_com[2];koll_unit_conver(Physical_to_Nominal,2);koll_test4_move();
		TargetTorque_ph[3]= model_compen[3]+Ff_com[3];koll_unit_conver(Physical_to_Nominal,3);koll_test4_move();
		TargetTorque_ph[4]= model_compen[4]+Ff_com[4];koll_unit_conver(Physical_to_Nominal,4);koll_test4_move();
		TargetTorque_ph[5]= model_compen[5]+Ff_com[5];koll_unit_conver(Physical_to_Nominal,5);koll_test4_move();
		TargetTorque_ph[6]= model_compen[6]+Ff_com[6];koll_unit_conver(Physical_to_Nominal,6);koll_test4_move();
	}
}

void koll_No_com()
{
    j1_pos_check();

    EnterMutex();
	OperationMode1 = 0x04;
	OperationMode2 = 0x04;
	OperationMode3 = 0x04;
	OperationMode4 = 0x04;
	OperationMode5 = 0x04;
	OperationMode6 = 0x04;
	LeaveMutex();

    while(1){
        if (operation_command == operation_stop) {
            break;
        }
        pthread_mutex_lock(&mut_operation);
        int status = opeation_status;
        pthread_mutex_unlock(&mut_operation);
        if (status == operation_finished){
            break;
        } 	
	
		TargetTorque_ph[1]= 0;koll_unit_conver(Physical_to_Nominal,1);koll_test4_move();
		TargetTorque_ph[2]= 0;koll_unit_conver(Physical_to_Nominal,2);koll_test4_move();
		TargetTorque_ph[3]= 0;koll_unit_conver(Physical_to_Nominal,3);koll_test4_move();
		TargetTorque_ph[4]= 0;koll_unit_conver(Physical_to_Nominal,4);koll_test4_move();
		TargetTorque_ph[5]= 0;koll_unit_conver(Physical_to_Nominal,5);koll_test4_move();
		TargetTorque_ph[6]= 0;koll_unit_conver(Physical_to_Nominal,6);koll_test4_move();
	}
}

void koll_test4_move()
 {
    EnterMutex(); 
    TargetTorque6=TargetTorque[1];
    TargetTorque5=TargetTorque[2];
    TargetTorque4=TargetTorque[3];
    TargetTorque3=TargetTorque[4];
    TargetTorque2=TargetTorque[5];
    TargetTorque1=TargetTorque[6];
    LeaveMutex();
} 

// 3.1-test5-运动到指定位置
void koll_test5_move()
 {
    TargetPosition_ph[1] = operation_postion[1];
    TargetPosition_ph[2] = operation_postion[2];
    TargetPosition_ph[3] = operation_postion[3];
    TargetPosition_ph[4] = operation_postion[4];
    TargetPosition_ph[5] = operation_postion[5];
    TargetPosition_ph[6] = operation_postion[6];

    koll_unit_conver(Physical_to_Nominal,0);

    j1_pos_move(abso_pos, TargetPosition_ph[1]);
    EnterMutex(); OperationMode5 = 0x01; TargetPosition5 = TargetPosition[2]; LeaveMutex();
    EnterMutex(); OperationMode4 = 0x01; TargetPosition4 = TargetPosition[3]; LeaveMutex();
    EnterMutex(); OperationMode3 = 0x01; TargetPosition3 = TargetPosition[4]; LeaveMutex();
    EnterMutex(); OperationMode2 = 0x01; TargetPosition2 = TargetPosition[5]; LeaveMutex();
    EnterMutex(); OperationMode1 = 0x01; TargetPosition1 = TargetPosition[6]; LeaveMutex();

    usleep(10*1000);
    pthread_mutex_lock(&mut_operation);
    opeation_status = operation_finished;
    pthread_mutex_unlock(&mut_operation);

} 

