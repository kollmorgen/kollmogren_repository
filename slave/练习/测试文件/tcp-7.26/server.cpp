#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>

using namespace std;
struct sockaddr_in addr;
struct sockaddr_in client;
int socket_fd;
int fd;

void* koll_recive(void* args)
{
    while(1){
    char *ip = inet_ntoa(client.sin_addr);
    char buffer[1024]={};
    int size = read(fd, buffer, sizeof(buffer));//通过fd与客户端联系在一起,返回接收到的字节数
    cout << "机械臂：【" << ip << "】反馈: "  << buffer << endl;
    }
}
void* koll_send(void* args)
{
    while(1){
        char buffsend[1024]; 
        fgets(buffsend,sizeof(buffsend),stdin);
        write(fd, buffsend, strlen(buffsend));
        cout <<"发布指令:" << buffsend << endl;
    }
 

}

int main()
{
    //1.创建一个socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    cout <<"socket_fd:" << socket_fd << endl;
    if (socket_fd == -1)
    {
        cout << "socket 创建失败： "<< endl;
        exit(1);
    }
    //2.准备通讯地址（必须是服务器的）192.168.1.100是本机的IP
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2040);//将一个无符号短整型的主机数值转换为网络字节顺序，即大尾顺序(big-endian)
    addr.sin_addr.s_addr = inet_addr("192.168.1.110");//net_addr方法可以转化字符串，主要用来将一个十进制的数转化为二进制的数，用途多于ipv4的IP转化。
 
    //3.bind()绑定
    //参数一：0的返回值（socket_fd）
    //参数二：(struct sockaddr*)&addr 前面结构体，即地址
    //参数三: addr结构体的长度


    int res = bind(socket_fd,(struct sockaddr*)&addr,sizeof(addr));
    if (res == -1)
    {
        cout << "bind创建失败： " << endl;
        exit(-1);
    }
    cout << "上位机准备就绪，等待机械臂连接" << endl;
    //4.监听客户端listen()函数
    //参数二：进程上限，一般小于30
    listen(socket_fd,30);

    //5.等待客户端的连接accept()，返回用于交互的socket描述符

    socklen_t len = sizeof(client);
    fd = accept(socket_fd,(struct sockaddr*)&client,&len);
  

    // while(1){
    //         if (fd == -1) 
    //         {
    //             cout << "accept错误\n" << endl;
    //             exit(-1);
    //         }
    //                    // 接收指令
    //          char *ip = inet_ntoa(client.sin_addr);
    //          cout << "机械臂：【" << ip << "】反馈: " ;
    //          char buffer[255]={};
    //          int size = read(fd, buffer, sizeof(buffer));//通过fd与客户端联系在一起,返回接收到的字节数
    //          cout << buffer << endl;
            
    //          write(fd, "welcome1", 7);  
    //         //发布指令
    //         cout <<"请发布指令:";
    //         char buffsend[1024]; 
    //         fgets(buffsend,sizeof(buffsend),stdin);
    //         write(fd, buffsend, strlen(buffsend));
    //         cout << endl;
 
    // }

    // 7.关闭sockfd
    pthread_exit(NULL);
    close(fd);
    close(socket_fd);
    return 0;
}


