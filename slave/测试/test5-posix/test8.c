#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

#define maxval(a,b) ((a>b)?a:b)

        int ret;
        int i = 0;
        timer_t timer;
        struct sigevent evp;
        struct timespec spec;
        struct itimerspec time_value;
   int a=0;
int k=0;



void function_timer()
{        
        //clock_gettime(CLOCK_MONOTONIC, &spec);
        //配置开启定时器后多久生效 开启后5s生效
       // time_value.it_value.tv_sec = spec.tv_sec + 0;     
        //time_value.it_value.tv_nsec = spec.tv_nsec + 1*1000000;
 a++;
if(a==1){
   printf("timeer:%d\n",k);a=0;k++;}
//clock_gettime(CLOCK_MONOTONIC, &spec);printf("6:%ld\n",spec.tv_sec);}
   //ret = timer_settime(timer, CLOCK_MONOTONIC , &time_value, NULL);
}



int main(int argc, const char *argv[])
{

        //evp.sigev_value.sival_ptr = &timer;

        //配置定时器，时间一到发送一个SIGUSR1信号，执行信号处理函数
       // evp.sigev_notify = SIGEV_SIGNAL;
        //evp.sigev_signo = SIGUSR1;
        //signal(SIGUSR1, function_timer);


	memset (&evp, 0, sizeof (struct sigevent));
	evp.sigev_value.sival_int = 0;
	evp.sigev_notify = SIGEV_THREAD;
	evp.sigev_notify_attributes = NULL;
	evp.sigev_notify_function = function_timer;

	if(timer_create (CLOCK_REALTIME, &evp, &timer)) {
		perror("timer_create()");
	}

        //创建定时器，时钟选择CLOCK_MONOTONIC，系统时钟的改变不会影响到定时器的执行
//clock_gettime(CLOCK_MONOTONIC, &spec);printf("3:%ld\n",spec.tv_sec);
       // ret = timer_create(CLOCK_MONOTONIC , &evp, &timer);
        //if( ret )
                //perror("timer_create");

        //配置多久执行一此  每秒执行一次
clock_gettime(CLOCK_MONOTONIC, &spec);printf("4:%ld\n",spec.tv_nsec);

	long tv_nsec = 1000 * (maxval(spec.tv_nsec,1)%1000000);
	time_t tv_sec = spec.tv_nsec/1000000;
	struct itimerspec timerValues;


        time_value.it_interval.tv_sec = 1;
        time_value.it_interval.tv_nsec = 1*1000000;
        //配置开启定时器后多久生效 开启后5s生效

        time_value.it_value.tv_sec = tv_sec;     
        time_value.it_value.tv_nsec =  tv_nsec;
        time_value.it_value.tv_sec = 3;     
        time_value.it_value.tv_nsec =  0;
        //设置开启定时器
       ret = timer_settime(timer, 0 , &time_value, NULL);
        if( ret )
                perror("timer_settime");

        while(1)
        {
                //打印时间戳
                ///printf("now running time:%d\n",i++);
                sleep(1);
printf("%ld,%ld,%ld,%ld\n",time_value.it_interval.tv_sec,time_value.it_interval.tv_nsec,time_value.it_value.tv_sec,time_value.it_value.tv_nsec);
//clock_gettime(CLOCK_MONOTONIC, &spec);printf("6:%ld\n",spec.tv_sec);
                if(i == 10000000000000)
                {//删除定时器
                    printf("delete timer new\n");
                        timer_delete(timer);
                        break;
                }
        }
        return 0;
}


