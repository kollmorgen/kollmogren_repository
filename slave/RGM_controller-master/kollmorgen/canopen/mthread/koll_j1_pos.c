#include"koll_j1_pos.h"
# include"koll_unit_conver.h"
#include "TestMaster.h"
#include "canfestival.h"
# include"koll_run.h"

struct _pid
{
  float target;             //定义设定值  
  float actual;             //定义实际值  
  float err;                //定义偏差值   
  float err_last;           //定义上一个偏差值  
  float Kp,Ki,Kd;           //定义比例、积分、微分系数  
  float operation;          //定义执行值  
  float integral;           //定义积分值
  double v_ph;           //速度
  long v;
} pid1_pos;


int j1_pos_target=0;
int j1_pidcount=0;

int J1_pos_status     = j1_close;  //j1_close的时候为线程结束状态,,j1_open的时候为线程开启状态
INTEGER32 J1_TargetPosition = 0x0; //当(J1_pos_status==1)的时候,只要这个数值改变,那么位置就改变!!!!!!!


// int ji_pos_thread_connect()
// void j1_pos_check()

int ji_pos_thread_connect() //开启线程+线程是否已经开启检查
{
    pthread_mutex_lock(&mut_j1_pos); //1-提取当前关节1运行的模式
    int sta = J1_pos_status; 
    pthread_mutex_unlock(&mut_j1_pos);
    if (sta==j1_close){             //2-如果处于关闭状态，则开启线程
	    int th_j1_pos=pthread_create(&pt_j1_pos,NULL, f_koll_j1_pos, NULL);
	    if (th_j1_pos != 0){
	    	printf("ERROR:fail to start j1 thread\n");
	    	return -1;
	    }
        pthread_mutex_lock(&mut_j1_pos);
        J1_pos_status = j1_open; 
        pthread_mutex_unlock(&mut_j1_pos);
    }
    return 0; //如果是开启状态则退出
}



//用于检查当前的状态是否已经退出了"J1_pos_status"！！！！也就是退出本线程！！！！
void j1_pos_check()
{
    // (1) 关闭位置模式
    pthread_mutex_lock(&mut_j1_pos);
    J1_pos_status = j1_close; 
    pthread_mutex_unlock(&mut_j1_pos);
    // (2) 默认进入速度控制模式 
    EnterMutex();    
    OperationMode6=3;
    TargetVelocity6=0;
    LeaveMutex();

}

// (1)每当使用位置模式的时候才会开启这个线程
void* f_koll_j1_pos(void* args)
{
    printf("[j1_pos线程]:已经进入\n");
    j1_pos_PID_init();
    // 进入电流模式
    EnterMutex();
    OperationMode6=4;
    LeaveMutex();

    while(1){
        int status=j1_pos_no_command;
        // (1)判断是否接收到指令
        pthread_mutex_lock(&mut_j1_pos);
        if (thread_j1pos_flag != 0) {
            status=thread_j1pos_flag;
            thread_j1pos_flag=0;
        }
        pthread_mutex_unlock(&mut_j1_pos);

        // (2)指令处理
        if(status==j1_pos_change){ /*位置模式下:驱动指令*/
            koll_unit_conver(Physical_to_Nominal,1);
            j1_pos_target= (float)TargetPosition[1];
        }
        else if(status==j1_pos_stop){/*位置模式下:停止指令*/
            EnterMutex();
            j1_pos_target=ActualPosition6;
            LeaveMutex(); 
        }
        else if(status==debug_j1_para){/*位置模式下:改变pid参数*/
            pthread_mutex_lock(&mut_tcp);
            pid1_pos.Kp = j1_kp; 
            pid1_pos.Ki = j1_ki;
            pid1_pos.Kd = j1_kd;
            pthread_mutex_unlock(&mut_tcp);
        }
        // (3)判断是否执行或退出
        pthread_mutex_lock(&mut_j1_pos);
        int sta = J1_pos_status; 
        pthread_mutex_unlock(&mut_j1_pos);
        if( sta == j1_open){
           j1_pos_run(j1_pos_target);
        }
        else
            break;
    }
    printf("[j1_pos线程]:已经退出\n");
    pthread_exit(NULL);
}



void j1_pos_run(float goal)
{
    usleep(5*1000);
    j1_pidcount++;
    EnterMutex();
    pid1_pos.actual=ActualPosition6;
    // pid1_pos.v=ActualVelocity6;
    LeaveMutex();  
    
    koll_unit_conver(Nominal_to_Physical,1);//单位转化
    pid1_pos.v_ph=ActualVelocity_ph[1];

	pid1_pos.target=goal; 
    pid1_pos.err=pid1_pos.target-pid1_pos.actual; 
    pid1_pos.integral+=pid1_pos.err;  

    pid1_pos.operation=pid1_pos.Kp*pid1_pos.err+pid1_pos.Ki*pid1_pos.integral+pid1_pos.Kd*(pid1_pos.err-pid1_pos.err_last);   //pid

    float error_vel=pid1_pos.err-pid1_pos.err_last;
	pid1_pos.err_last=pid1_pos.err;

    if(pid1_pos.operation>0) pid1_pos.operation=pid1_pos.operation+800; //摩擦力补偿
	if(pid1_pos.operation<0) pid1_pos.operation=pid1_pos.operation-800; 

    if(pid1_pos.operation>= 2400  ) pid1_pos.operation=2400; //电流限制
	if(pid1_pos.operation<=-2400 ) pid1_pos.operation=-2400; 

    // if(pid1_pos.operation>= 6000 && pid1_pos.v_ph>=7 ) pid1_pos.operation=6000; 
	// if(pid1_pos.operation<=-6000 && pid1_pos.v_ph>=7) pid1_pos.operation=-6000; 

	if(pid1_pos.operation>=2400 && pid1_pos.v_ph>=10) pid1_pos.operation=2400; 　//速度限制
	if(pid1_pos.operation<=-2400 && pid1_pos.v_ph<=-10) pid1_pos.operation=-2400; 
    
    // printf("速度：%lf\n",pid1_pos.v_ph);
    
    if(fabs(pid1_pos.err)<100){ //死区
        pid1_pos.err=0;
    }
    //if(j1_pidcount==100){
        //printf("当前的数值：P：%f,I:%f,D:%f,最终输出：%f,速度偏差：%f\n",pid1_pos.Kp,pid1_pos.Ki,pid1_pos.Kd,pid1_pos.operation,error_vel);



    //    printf("位置偏差:%f  速度偏差:%f  输出数据:%f\n",pid1_pos.err,error_vel,pid1_pos.operation);



	//	j1_pidcount=0;
    //}


    EnterMutex();
    TargetTorque6= pid1_pos.operation;
    LeaveMutex();
}



void j1_pos_PID_init()
{  pid1_pos.Kp=1.8; 
  pid1_pos.Ki=0.0;
  pid1_pos.Kd=12;//2 0 30
  /*初始化设置*/ 
  pid1_pos.target=0.0; 
  pid1_pos.actual=0.0;
  pid1_pos.err=0.0; 
  pid1_pos.err_last=0.0; 
  pid1_pos.operation=0.0; 
  pid1_pos.integral=0.0; 
  /*PID参数*/
  pid1_pos.Kp=1.8; 
  pid1_pos.Ki=0.0;
  pid1_pos.Kd=12;//2 0 30
  }

