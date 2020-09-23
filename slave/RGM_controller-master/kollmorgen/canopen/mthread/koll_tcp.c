#include "koll_tcp.h"
#include "kollmorgen.h"
#include "koll_run.h"
# include"koll_unit_conver.h"

struct sockaddr_in addr;
int socket_koll;
pthread_mutex_t mute;

R_Command r_cmd;
S_Command s_cmd; 

//定义一些多个线程之间的传递数据
double test2_num;
float j1_kp, j1_kd, j1_ki;

// (1)thread_tcp:main
void* f_koll_tcp(void* args)
{
    int ret1 = f_koll_tcp_init();

    pthread_mutex_lock(&mut_tcp);
    thread_tcp_flag=ret1;
    pthread_mutex_unlock(&mut_tcp);

    if( ret1 == 0){
        f_koll_tcp_comm();
    }
    f_koll_tcp_stop();

    pthread_mutex_lock(&mut_tcp);
    thread_tcp_flag=Command_Shutdown;
    pthread_mutex_unlock(&mut_tcp);
    
    pthread_exit(NULL);
}

// (2)thread_tcp:init
int f_koll_tcp_init()
{
    if ((socket_koll = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
        return -1;
    }
    memset(&addr, 0, sizeof(addr));           // server address
    addr.sin_family = AF_INET;                       // Protocol Family 
    addr.sin_port = htons(KOLL_PORT);                     // Port number 
    if( inet_pton(AF_INET, KOLL_SERVER_ADDR, &addr.sin_addr) <= 0){
        printf("inet_pton error for %s\n",KOLL_SERVER_ADDR);
        return -2;
    } 
    int res;
    if( res=connect(socket_koll, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno); 
        return -3;
    }
    else
    {
        printf ("[tcp线程]:成功连接到: %s\n",KOLL_SERVER_ADDR);
        return 0;
    }
}
 

// (3)thread_tcp:communication
int f_koll_tcp_comm()
{
    while(1)
    {
        memset(s_cmd.k_command, 0, sizeof s_cmd.k_command);

        int size = read(socket_koll, &r_cmd, sizeof(R_Command));//通过fd与客户端联系在一起,返回接收到的字节数
        
        //printf("[TCP]:[Ucp]:%c-%s\n",r_cmd.c_index, r_cmd.k_command);

        if (r_cmd.c_index == '0')
        {
            s_cmd.c_index = '0';
            strcpy(s_cmd.k_command,"Can't understand!");
            //printf("[TCP]:[Lcp]:index: %c, command: %s\n",s_cmd.c_index, s_cmd.k_command);
            write(socket_koll, &s_cmd, sizeof(S_Command));
        }
        else if(r_cmd.c_index == '1')
        {
            s_cmd.c_index = r_cmd.c_index;    

            if (!strncmp(r_cmd.k_command,"Shutdown",8))
            {            
                //printf("[TCP]:[Lcp]:index: %c, command: %s\n",r_cmd.c_index, r_cmd.k_command);
                return 0;
            }
            else if (!strncmp(r_cmd.k_command,"Start",5))
            {
                strcpy(s_cmd.k_command,"Start-OK"); 
                printf("[TCP]:[Lcp]:index: %c, command: %s\n",s_cmd.c_index, s_cmd.k_command);
                write(socket_koll, &s_cmd, sizeof(S_Command));
            }
            else if (!strncmp(r_cmd.k_command,"Stop",4))
            {
                turn_to_run_thread(Command_stop);
                strcpy(s_cmd.k_command,"Stop-OK"); 
                printf("[TCP]:[Lcp]:index: %c, command: %s\n",s_cmd.c_index, s_cmd.k_command);
                write(socket_koll, &s_cmd, sizeof(S_Command));
            }
            else 
            {
                Nocommand();
            }
        }
        else if (r_cmd.c_index == '2')//电机测试模式
        {
            //S_cmd message
            int n_2 = strlen(r_cmd.k_command);
            strncpy(s_cmd.k_command, r_cmd.k_command, n_2);
            s_cmd.k_command[n_2] = '-';
            s_cmd.k_command[n_2+1] = 'O';
            s_cmd.k_command[n_2+2] = 'K';
            s_cmd.c_index = r_cmd.c_index;
            //control cmd
            int motor = r_cmd.k_command[n_2-1] - 48;//电机号

            write(socket_koll, &s_cmd, sizeof(S_Command));
            //printf("[TCP]:[Lcp]:index: %c, command: %s\n",r_cmd.c_index, s_cmd.k_command);

            if(n_2 == 2)
            {
                if(r_cmd.k_command[0] == 's')//电机停止
                {                
                    switch(motor){
                        case 1:{turn_to_run_thread(Test_M1_s1);}break;
                        case 2:{turn_to_run_thread(Test_M1_s2);}break;
                        case 3:{turn_to_run_thread(Test_M1_s3);}break;
                        case 4:{turn_to_run_thread(Test_M1_s4);}break;
                        case 5:{turn_to_run_thread(Test_M1_s5);}break;
                        case 6:{turn_to_run_thread(Test_M1_s6);}break;
                    }
                }
                else//电机正转
                {
                    switch(motor){
                        case 1:{turn_to_run_thread(Test_M1_q1);}break;
                        case 2:{turn_to_run_thread(Test_M1_q2);}break;
                        case 3:{turn_to_run_thread(Test_M1_q3);}break;
                        case 4:{turn_to_run_thread(Test_M1_q4);}break;
                        case 5:{turn_to_run_thread(Test_M1_q5);}break;
                        case 6:{turn_to_run_thread(Test_M1_q6);}break;
                    }
                }
            }
            else if(n_2 == 3)//反转
            {
                switch(motor){
                    case 1:{turn_to_run_thread(Test_M1_qq1);}break;
                    case 2:{turn_to_run_thread(Test_M1_qq2);}break;
                    case 3:{turn_to_run_thread(Test_M1_qq3);}break;
                    case 4:{turn_to_run_thread(Test_M1_qq4);}break;
                    case 5:{turn_to_run_thread(Test_M1_qq5);}break;
                    case 6:{turn_to_run_thread(Test_M1_qq6);}break;
                }
            }

        }
        else if (r_cmd.c_index == '3')//位置/速度/电流控制
        {
            s_cmd.c_index = r_cmd.c_index;
            strncpy(s_cmd.k_command, r_cmd.k_command, 5);
            s_cmd.k_command[5] = 'O';
            s_cmd.k_command[6] = 'K';

            write(socket_koll, &s_cmd, sizeof(R_Command));
           // printf("[TCP]:[Lcp]:index: %c, command: %s\n",r_cmd.c_index, s_cmd.k_command);

            char char_float[30] = "";
            double control_num = 0.0;
            strcpy(char_float, &r_cmd.k_command[5]);
            control_num = (double)atof(char_float);//控制量
            printf("[TCP]:[Lcp]:control mode: %c, control quantity: %f\n",r_cmd.k_command[3], control_num);
            
            int motor = r_cmd.k_command[1]-48;//电机号

        	pthread_mutex_lock(&mut_tcp);
            test2_num = control_num;
            pthread_mutex_unlock(&mut_tcp);

            if(r_cmd.k_command[3] == 'p')//位置
            {           
                switch(motor){
                    case 1:{turn_to_run_thread(Test_M2_q1_P);}break;
                    case 2:{turn_to_run_thread(Test_M2_q2_P);}break;
                    case 3:{turn_to_run_thread(Test_M2_q3_P);}break;
                    case 4:{turn_to_run_thread(Test_M2_q4_P);}break;
                    case 5:{turn_to_run_thread(Test_M2_q5_P);}break;
                    case 6:{turn_to_run_thread(Test_M2_q6_P);}break;
                }
            }
            else if(r_cmd.k_command[3] == 'v')//速度
            {
                switch(motor){
                    case 1:{turn_to_run_thread(Test_M2_q1_V);}break;
                    case 2:{turn_to_run_thread(Test_M2_q2_V);}break;
                    case 3:{turn_to_run_thread(Test_M2_q3_V);}break;
                    case 4:{turn_to_run_thread(Test_M2_q4_V);}break;
                    case 5:{turn_to_run_thread(Test_M2_q5_V);}break;
                    case 6:{turn_to_run_thread(Test_M2_q6_V);}break;
                }
            }
            else if(r_cmd.k_command[3] == 'f')//电流
            {
                switch(motor){
                    case 1:{turn_to_run_thread(Test_M2_q1_T);}break;
                    case 2:{turn_to_run_thread(Test_M2_q2_T);}break;
                    case 3:{turn_to_run_thread(Test_M2_q3_T);}break;
                    case 4:{turn_to_run_thread(Test_M2_q4_T);}break;
                    case 5:{turn_to_run_thread(Test_M2_q5_T);}break;
                    case 6:{turn_to_run_thread(Test_M2_q6_T);}break;
                }
            }
        }
        else if (r_cmd.c_index == '4')//激励轨迹
        {
            int n_4 = strlen(r_cmd.k_command);
            s_cmd.c_index = r_cmd.c_index;
            strcpy(s_cmd.k_command, r_cmd.k_command);
            s_cmd.k_command[n_4] = '-';
            s_cmd.k_command[n_4+1] = 'O';
            s_cmd.k_command[n_4+2] = 'K';

            write(socket_koll, &s_cmd, sizeof(R_Command));
           // printf("[TCP]:[Lcp]:index: %c, command: %s\n",r_cmd.c_index, s_cmd.k_command);
            
            if(r_cmd.k_command[n_4-1] == '1')
            {
                turn_to_run_thread(Test_M3_run1);//激励轨迹1控制
            }
            else if(r_cmd.k_command[n_4-1] == '2')
            {
                turn_to_run_thread(Test_M3_run2);//激励轨迹2控制
            }
            else if(r_cmd.k_command[n_4-1] == '3')
            {
                turn_to_run_thread(Test_M3_run3);//激励轨迹3控制
            }
            else if(r_cmd.k_command[n_4-1] == '4')
            {
                turn_to_run_thread(Test_M3_run4);//激励轨迹4控制
            }

        }
        else if (r_cmd.c_index == '5')//零力拖动
        {
            int n_5 = strlen(r_cmd.k_command);
            s_cmd.c_index = r_cmd.c_index;
            strcpy(s_cmd.k_command, r_cmd.k_command);
            s_cmd.k_command[n_5] = '-';
            s_cmd.k_command[n_5+1] = 'O';
            s_cmd.k_command[n_5+2] = 'K';

            write(socket_koll, &s_cmd, sizeof(R_Command));
          //  printf("[TCP]:[Lcp]:index: %c, command: %s\n",r_cmd.c_index, s_cmd.k_command);
            
            if(r_cmd.k_command[0] == 'N')
            {
                turn_to_run_thread(Test_M4_no);//无补偿
            }
            else if(r_cmd.k_command[0] == 'G')
            {
                turn_to_run_thread(Test_M4_G);//重力补偿
            }
            if(r_cmd.k_command[0] == 'F')
            {
                turn_to_run_thread(Test_M4_GF);//重力+摩擦力补偿
            }

        }
        else if (r_cmd.c_index == '6')//示教
        {
            int n_6 = strlen(r_cmd.k_command);
            s_cmd.c_index = r_cmd.c_index;
            strcpy(s_cmd.k_command, r_cmd.k_command);
            s_cmd.k_command[n_6] = '-';
            s_cmd.k_command[n_6+1] = 'O';
            s_cmd.k_command[n_6+2] = 'K';

            write(socket_koll, &s_cmd, sizeof(R_Command));
          //  printf("[TCP]:[Lcp]:index: %c, command: %s\n",r_cmd.c_index, s_cmd.k_command);
            
            if(r_cmd.k_command[0] == 'P'){
                if(r_cmd.k_command[1] == 'i'){
                    turn_to_run_thread(Test_M5_pos_init);//移动到初始位置
                }
                else if(r_cmd.k_command[1] == 'd'){
                    turn_to_run_thread(Test_M5_pos_identif);//移动到激励轨迹位置
                }
                
            }
            else if(r_cmd.k_command[0] == 'M'){//标记点
                if(r_cmd.k_command[n_6-1] == '1'){
                    turn_to_run_thread(Test_M5_mark1);//记录标记点1
                }
                else if(r_cmd.k_command[n_6-1] == '2'){
                    turn_to_run_thread(Test_M5_mark2);//记录标记点2
                }
                else if(r_cmd.k_command[n_6-1] == '3'){
                    turn_to_run_thread(Test_M5_mark3);//记录标记点3
                }
            }
            else if(r_cmd.k_command[0] == 'R'){//标记点
                if(r_cmd.k_command[n_6-1] == '1'){
                    turn_to_run_thread(Test_M5_run_to_mark1);//移动到记录标记点1
                }
                else if(r_cmd.k_command[n_6-1] == '2'){
                    turn_to_run_thread(Test_M5_run_to_mark2);//移动到记录标记点2
                }
                else if(r_cmd.k_command[n_6-1] == '3'){
                    turn_to_run_thread(Test_M5_run_to_mark3);//移动到记录标记点3
                }
            }

        }
        else if (r_cmd.c_index == 'P')//PID调试
        {
            float Kp = r_cmd.motor_PID[0];
            float Ki = r_cmd.motor_PID[1];
            float Kd = r_cmd.motor_PID[2];

            pthread_mutex_lock(&mut_tcp);
            j1_kp = Kp ;
            j1_kd = Kd ; 
            j1_ki = Ki ;           
            pthread_mutex_unlock(&mut_tcp);
            turn_to_run_thread(debug_j1_para);

            s_cmd.c_index = r_cmd.c_index;
            strcpy(s_cmd.k_command,"PID-OK"); 
           // printf("[TCP]:[Lcp]:index: %c, command: %s\n",s_cmd.c_index, s_cmd.k_command);
            write(socket_koll, &s_cmd, sizeof(S_Command));

            printf("[TCP]:[Lcp]:PID: Kp: %f, Ki: %f, Kd: %f, \n", Kp, Ki, Kd);
        }
        else if (r_cmd.c_index == 'D')//返回电机数据
        {
            int i = 0;
            koll_unit_conver(Nominal_to_Physical ,0);

            // pthread_mutex_lock(&mut_run); 
		    // ActualPosition_ph[1]=ActualPosition6;
		    // ActualPosition_ph[2]=ActualPosition5;
		    // ActualPosition_ph[3]=-ActualPosition4;
		    // ActualPosition_ph[4]=ActualPosition3;
		    // ActualPosition_ph[5]=ActualPosition2;
		    // ActualPosition_ph[6]=ActualPosition1;
            // pthread_mutex_unlock(&mut_run);

            double motor_postion[] = {ActualPosition_ph[1],ActualPosition_ph[2],ActualPosition_ph[3],ActualPosition_ph[4],ActualPosition_ph[5],ActualPosition_ph[6]};
            double motor_velocity[]= {ActualVelocity_ph[1],ActualVelocity_ph[2],ActualVelocity_ph[3],ActualVelocity_ph[4],ActualVelocity_ph[5],ActualVelocity_ph[6]};
            double motor_current[] = {  ActualTorque_ph[1],  ActualTorque_ph[2],  ActualTorque_ph[3],  ActualTorque_ph[4],  ActualTorque_ph[5],  ActualTorque_ph[6]};
            double force_sensor[] =  {1.1,2.2,3.3,4.4,5.5,6.6};
   
            //printf("这里是位置：%2.2lf-%2.2lf-%2.2lf-%2.2lf-%2.2lf-%2.2lf\n",motor_postion[0],motor_postion[1],motor_postion[2],motor_postion[3],motor_postion[4],motor_postion[5]);
            //printf("这里是速度：%2.2lf-%2.2lf-%2.2lf-%2.2lf-%2.2lf-%2.2lf\n",motor_velocity[0],motor_velocity[1],motor_velocity[2],motor_velocity[3],motor_velocity[4],motor_velocity[5]);
            s_cmd.c_index = r_cmd.c_index;
            strcpy(s_cmd.k_command, "Motor Data-OK");
            strcpy(s_cmd.data_status, "Displaying Motor Data"); 

            for(i=0; i<6; i++)
            {
                s_cmd.data_pos[i] = motor_postion[i];
                s_cmd.data_vel[i] = motor_velocity[i];
                s_cmd.data_cur[i] = motor_current[i];
                s_cmd.data_fsensor[i] = force_sensor[i];
            }

            write(socket_koll, &s_cmd, sizeof(S_Command));
            //printf("[TCP]:[Lcp]:index: %c, command: %s\n",s_cmd.c_index, s_cmd.k_command);

        }
        else
        {
            Nocommand();
        }


        init_r_command(&r_cmd);
        init_s_command(&s_cmd);
        
   }


   return 0;
}


// (4)thread_tcp:stop
int f_koll_tcp_stop()
{
    close(socket_koll);
    printf("[tcp线程]: Has closed TCP-thread\n");
    return 0;
}




void Nocommand(void)
{
    s_cmd.c_index = '0';
    strcpy(s_cmd.k_command,"Can't understand!");
    printf("[TCP]:[Lcp]:index: %c, command: %s\n",s_cmd.c_index, s_cmd.k_command);
    write(socket_koll, &s_cmd, sizeof(S_Command));
}

void init_r_command(R_Command * kollSstruct)
{
    kollSstruct->c_index = '0';
    strcpy(kollSstruct->k_command, "");
    kollSstruct->motor_PID[0] = 0.0;
    kollSstruct->motor_PID[1] = 0.0;
    kollSstruct->motor_PID[2] = 0.0;
}


void init_s_command(S_Command * kollstruct)
{
    int i=0;
    kollstruct->c_index = '0';
    strcpy(kollstruct->k_command, "");
    strcpy(kollstruct->data_status, "");
    for(i=0; i<6; i++)
    {
        kollstruct->data_cur[i]=0.0;
        kollstruct->data_vel[i]=0.0;
        kollstruct->data_pos[i]=0.0;
        kollstruct->data_fsensor[i]=0.0;
    }
}

void turn_to_run_thread(int command)
{
    pthread_mutex_lock(&mut_run);
    thread_run_flag=command;
    pthread_mutex_unlock(&mut_run);

}