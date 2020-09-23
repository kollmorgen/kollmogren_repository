#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

#define N 100   //定义队列最大多少
#define datatype char  //定义队列的数据类型

struct queue
{
    datatype data[N][20];//保存数据的数组
    int front;  //数据的开头
    int rear;  //数据的结尾
};
typedef struct queue Q; //给已经有的类型简化一下

/*
void init(Q *myqueue); //初始化结构体
int isempty(Q *myqueue); //判断是否为空，1为空，0不为空
void enQueue(Q *myqueue,datatype num);  //入队
datatype DeQueue(Q *myqueue);//出队
void printfQ(Q *myqueue); //打印队列所有的元素
datatype gethead(Q *myqueue);//获取开头的一个节点
*/


//初始化结构体
int init(Q *myqueue)
{
    myqueue->front = 0;
    myqueue->rear = 0;
    return 1;
}
//判断是否为空，1为空，0不为空
int clearqueue(Q *myqueue)
{

    myqueue->front = 0;
    myqueue->rear = 0;
    return 1; 
}
int isempty(Q *myqueue)
{
    if (myqueue->front==myqueue->rear)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//入队
int enQueue(Q *myqueue, const datatype *num)
{
    if (myqueue->rear == N)
    {
        printf("队列已满\n");
        return -1;
    }
    else
    {
        // myqueue->data[myqueue->rear] = num;//赋值
        strcpy(myqueue->data[myqueue->rear],num);
        myqueue->rear += 1;//增加一个
        return 1;
    }
}
//出队
datatype DeQueue(Q *myqueue)
{
    if (myqueue->front==myqueue->rear)
    {
        printf("失败");
        return -1;
    }
    else
    {
        myqueue->front += 1;
        //return myqueue->data[myqueue->front-1];
    }
}
//打印队列所有的元素
void printfQ(Q *myqueue) //打印队列所有的元素
{
    printf("\n");
    if (myqueue->front==myqueue->rear)
    {
        printf("为空");
    }
    else
    {
        for (int i = myqueue->front; i < myqueue->rear; i++)
        {
            printf("%6s",myqueue->data[i]);
            float aa=atoi(myqueue->data[i]);
            printf("(%2.4f)",aa);
        }
    }
}
//获取开头的一个节点
datatype gethead(Q *myqueue)
{
    if (myqueue->front == myqueue->rear)
    {
        printf("为空");
        return -1;
    }
    else
    {
        //return myqueue->data[myqueue->front];
    }
}
int main()
{
    Q Q1;
    init(&Q1);
    char a[10]="12.33";
    char b[10]="1";
    char c[10]="2";
    char d[10]="4.5";
    char e[10]="43";
    enQueue(&Q1,a);printfQ(&Q1);
    enQueue(&Q1,b);printfQ(&Q1);
    enQueue(&Q1,c);printfQ(&Q1);
    enQueue(&Q1,d);printfQ(&Q1);
    enQueue(&Q1,e);printfQ(&Q1);

    DeQueue(&Q1);printfQ(&Q1);
    DeQueue(&Q1);printfQ(&Q1);
    DeQueue(&Q1);printfQ(&Q1);
    DeQueue(&Q1);printfQ(&Q1);
    DeQueue(&Q1);printfQ(&Q1);
    DeQueue(&Q1);printfQ(&Q1);
    DeQueue(&Q1);printfQ(&Q1);

    system("pause");
}