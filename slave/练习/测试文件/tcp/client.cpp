#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include <pthread.h>


using namespace std;
struct sockaddr_in addr;
#define NUM_THREADS 2
int socket_fd;

void* koll_recive(void* printf)
{
    while(1){
    char buffer[1024]={};
    int size = read(socket_fd, buffer, sizeof(buffer));//通过fd与客户端联系在一起,返回接收到的字节数
    cout << "反馈: "  << buffer << endl;
    }
    //   while(1){
    //     char buf[100] = {};
    //     if (recv(socket_fd,buf,sizeof(buf),0) <= 0){
    //         cout<<"错误"<<endl;
    //     }
    //     printf("%s\n",buf);
    // }
}
void* koll_send(void* args)
{
    while(1){
        char buffsend[1024]; 
        fgets(buffsend,sizeof(buffsend),stdin);
        write(socket_fd, buffsend, strlen(buffsend));
       // cout <<"发布指令:" << buffsend << endl;
    }
 

}

int main()
{
        void* koll_recive(void*);
       //0.创建多线程任务
    pthread_t tids[NUM_THREADS];
    int socket_fd = socket(AF_INET, SOCK_STREAM,0);
    if(socket_fd == -1)
    {
        cout<<"socket 创建失败："<<endl;
        exit(-1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2040);
    addr.sin_addr.s_addr = inet_addr("192.168.1.110");

    int res = connect(socket_fd,(struct sockaddr*)&addr,sizeof(addr));
    if(res == -1)
    {
        cout<<"bind 连接失败："<<endl;
        exit(-1);
    }
    cout<<"bind 连接成功："<<endl;

//   while(1){
    //     char buf[100] = {};
    //     if (recv(socket_fd,buf,sizeof(buf),0) <= 0){
    //         cout<<"错误"<<endl;
    //     }
    //     printf("%s\n",buf);
    // }
    int ret  = pthread_create(&tids[0], NULL, koll_send, NULL);
    //int ret2 = pthread_create(&tids[1], NULL, koll_recive, NULL);
  
    pthread_exit(NULL);
    close(socket_fd);

    return 0;
}

