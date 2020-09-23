#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include<pthread.h>
#include<string.h>

//test



using namespace std;
#define NUM_THREADS 2
struct sockaddr_in addr;
struct sockaddr_in client;
int socket_fd;
pthread_mutex_t mute;
string Motor_Command = "";
// pthread_mutex_lock(&mute);
// pthread_mutex_unlock(&mute);

typedef struct Koll_Receive_Command_struct {
    char c_index;                      //Command index
    char k_command[30];
    char data_status[20];
    float data_pos[6];
    float data_vel[6];
    float data_cur[6];
    float data_fsensor[6];
} R_Command ;

typedef struct Koll_Send_Command_struct {
    char c_index;                      //Command index
    char k_command[30];
} S_Command;


R_Command s_cmd;
S_Command r_cmd;

void* koll_recive(void* args)
{
    while(1){
    //char buffer[255]={};
    int size = read(socket_fd, &r_cmd, sizeof(S_Command));
    cout << "上位机反馈:" << r_cmd.c_index << endl << r_cmd.k_command[0]<<endl;
    }
}

void* koll_send(void* args)
{
    while(1){
//        char buffsend[1024];
//        fgets(buffsend,sizeof(buffsend),stdin);
        strcpy(s_cmd.k_command, "Get It!");
        write(socket_fd, &s_cmd, sizeof(R_Command));
        cout <<"发布指令:" << s_cmd.k_command[1] << endl;
    }

}

int main()
{
    pthread_t tids[NUM_THREADS];
    pthread_mutex_init(&mute,NULL);

    int socket_fd = socket(AF_INET, SOCK_STREAM,0);
    cout << "socket_fd:" << socket_fd << endl;
    if(socket_fd == -1)
    {
        cout<<"socket 创建失败："<<endl;
        exit(-1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2040);
    addr.sin_addr.s_addr = inet_addr("192.168.1.130");

    int res = connect(socket_fd,(struct sockaddr*)&addr,sizeof(addr));
    if(res == -1)
    {
        cout<<"bind 连接失败："<<endl;
        exit(-1);
    }
    cout<<"bind 连接成功："<<endl;
    s_cmd.c_index = '9';strcpy(s_cmd.k_command, "Get It!");
    float ttt = 0.01;
   while(1){
    char bufcli[1024];
    char buffer[255]={};
    int size = read(socket_fd, &r_cmd, sizeof(S_Command));//通过fd与客户端联系在一起,返回接收到的字节数

    cout << "上位机指令:"<< buffer << endl;
    if (strcmp(buffer,"stop")==0)  break;
    else if (strcmp(r_cmd.k_command,"Start")==0)  {char buffre[1024];strcpy(buffre,"Start OK");  cout << "回复:"<< buffre << endl;write(socket_fd, &s_cmd, sizeof(R_Command));}
    else if (r_cmd.c_index == 'D')
    {
        cout<<"c_index: "<<r_cmd.c_index<<endl;
        s_cmd.c_index = 'D';
        strcpy(s_cmd.data_status, "Get the Data");
        for(int i=0; i<6; i++)
        {
            s_cmd.data_fsensor[i] = i*ttt;
            s_cmd.data_pos[i] = i*ttt*2;
            s_cmd.data_vel[i] = i*ttt*3;
            s_cmd.data_cur[i] = i*ttt*4;
        }
        write(socket_fd, &s_cmd, sizeof(R_Command));

        cout<<"send message index: "<<s_cmd.c_index<<endl;
        printf("send motor data\n");
    }
    else if (r_cmd.c_index == '2')
    {
        //S_cmd
        int n = strlen(r_cmd.c_index);
        strncpy(s_cmd.k_command, r_cmd.k_command, n);
        s_cmd.k_command[n] = 'O';
        s_cmd.k_command[n+1] = 'K';

        s_cmd.c_index = r_cmd.c_index;

        //control cmd
        int motor = r_cmd.k_command[n-1] - 48;    //电机号

        write(socket_fd, &s_cmd, sizeof(R_Command));

        cout<<"send message index: "<<s_cmd.c_index<<endl;
        printf("send message cmd: %s\n", s_cmd.k_command);

    }
    else if (r_cmd.c_index == '3')
    {
        cout<<"c_index: "<<r_cmd.c_index<<endl;
        s_cmd.c_index = r_cmd.c_index;
//        r_cmd.k_command[3] control mode
        char char_float[30] = "";
        float control_num = 0.0;
        int motor = r_cmd.k_command[1]-48;

        strcpy(char_float, &r_cmd.k_command[5]);
        control_num = atof(char_float);
        printf("send float: %f\n", control_num);

        printf("send command: %s\n", r_cmd.k_command);

        if(strncmp(r_cmd.k_command,"M1", 2) == 0)
        {
            strncpy(s_cmd.k_command, r_cmd.k_command, 5);
            s_cmd.k_command[5] = 'O';
            s_cmd.k_command[6] = 'K';
        }
        printf("send message: %s", s_cmd.k_command);
        cout<<"send is gonna ok"<<endl;
        write(socket_fd, &s_cmd, sizeof(R_Command));
        cout<<"send ok"<<endl;

    }
    else  {char buffre[1024];strcpy(buffre,"Can't understand!"); cout << "回复:"<< buffre << endl; write(socket_fd, buffre, strlen(buffre));}
     
   }
      //int ret  = pthread_create(&tids[0], NULL, koll_send, NULL);
      //int ret2 = pthread_create(&tids[1], NULL, koll_recive, NULL);


     pthread_exit(NULL);
     pthread_mutex_destroy(&mute);
     close(socket_fd);

    return 0;
}
















































