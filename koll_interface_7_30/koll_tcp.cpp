# include "koll_tcp.h"
# include <QString>
# include "koll_interface_7_30.h"

using namespace std;

struct sockaddr_in kolladdr;
struct sockaddr_in client;
int tcpsocketping;
int socket_fd;
int fd;
int thread;
pthread_mutex_t mut, kkl,mut_rec;
int kk=0;
extern S_Command *s_cmd;
extern R_Command *r_cmd;
extern R_Command *r_cmd_tcp_swap;

void* koll_recive(void* args)
{
    while(thread){
            int size = read(fd, r_cmd, sizeof(R_Command));//通过fd与客户端联系在一起,返回接收到的字节数

            pthread_mutex_lock(&mut_rec);
            memcpy(r_cmd_tcp_swap, r_cmd, sizeof(R_Command));
            init_r_command(r_cmd);
            qDebug()<<"Reci_message: "<< r_cmd_tcp_swap->c_index<<"-"<<r_cmd_tcp_swap->k_command;
//            qDebug()<<"r_cmd: "<<r_cmd->c_index;

            pthread_mutex_unlock(&mut_rec);
    }
    pthread_exit(NULL);
}

void* koll_send(void* args)
{
    while(thread){
        pthread_mutex_lock(&mut);

        if (s_cmd->c_index != '0') {
           // qDebug()<<"send message: "<<sizeof (S_Command);
            qDebug()<<"Send_message: "<< s_cmd->c_index<<"-"<<s_cmd->k_command;
            write(fd, s_cmd, sizeof(S_Command));
            init_s_command(s_cmd);
        }

        pthread_mutex_unlock(&mut);
    }

    pthread_exit(NULL);
}
int kolltcp_init()
{
    //0.创建多线程任务
    pthread_t tids[NUM_THREADS];
    //1.创建一个socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    cout <<"socket_fd:" << socket_fd << endl;
    if (socket_fd == -1)
    {
        cout << "socket 创建失败： "<< endl;
        return -1;
    }
    //2.准备通讯地址（必须是服务器的）192.168.1.110是本机的IP
    kolladdr.sin_family = AF_INET;
    kolladdr.sin_port = htons(2040);//将一个无符号短整型的主机数值转换为网络字节顺序，即大尾顺序(big-endian)
    kolladdr.sin_addr.s_addr = inet_addr(TCPSAERVER_ADDR);//net_addr方法可以转化字符串，主要用来将一个十进制的数转化为二进制的数，用途多于ipv4的IP转化。
 
    //3.bind()绑定
    int res = bind(socket_fd,(struct sockaddr*)&kolladdr,sizeof(kolladdr));
    if (res == -1)
    {
        cout << "bind创建失败： " << endl;
        return -2;
    }
    cout << "bind ok 等待客户端的连接" << endl;
    QTime t;
    t.start();
    while(t.elapsed()<10);
    system("gnome-terminal -x bash -c 'ssh root@192.168.1.100 '");
    //system("gnome-terminal -x bash -c 'ssh root@192.168.1.100 './kollmorgen' '");
    QTime t1;
    t1.start();
    while(t1.elapsed()<10);

    //4.监听客户端listen()函数  参数二：进程上限，一般小于30
    listen(socket_fd,30);
    //5.等待客户端的连接accept()，返回用于交互的socket描述符
    socklen_t len = sizeof(client);
    fd = accept(socket_fd,(struct sockaddr*)&client,&len);

    //6.开启多线程任务
    thread=1;
    int ret1=pthread_create(&tids[0], NULL, koll_send, NULL);
    int ret2=pthread_create(&tids[1], NULL, koll_recive, NULL);
    if(ret1!=0) return -3;
    if(ret2!=0) return -4;
    return 0;
  
}


int kolltcp_close()
{
    pthread_mutex_lock(&kkl);
    thread=0;
    pthread_mutex_unlock(&kkl);

//    char buffsend[1024];
//    strcpy(buffsend,"Shutdown");
//    write(fd, buffsend, strlen(buffsend));
//    cout <<"发布指令:" << buffsend << endl;
    close(fd);
    close(socket_fd);
    return 0;
}

int kolltcp_ping ( ) {
    tcpsocketping = socket(AF_INET, SOCK_DGRAM, 0);
    if (tcpsocketping < 0) {
        return -1;
    }
    kolladdr.sin_family = AF_INET;
    kolladdr.sin_port = htons(2040);//将一个无符号短整型的主机数值转换为网络字节顺序，即大尾顺序(big-endian)
    kolladdr.sin_addr.s_addr = inet_addr(TCPSAERVER_ADDR);//net_addr方法可以转化字符串，主要用来将一个十进制的数转化为二进制的数，用途多于ipv4的IP转化。
    int res = bind(tcpsocketping,(struct sockaddr*)&kolladdr,sizeof(kolladdr));
    if (res == -1)
    {
        return -2;
    }
    close(tcpsocketping);
    return 0;
}
