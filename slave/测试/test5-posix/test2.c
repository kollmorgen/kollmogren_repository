/*************************************************************************
    > File Name       : posix_timer.c
    > Author          : Jalyn
    > Mail            : JalynFang@outlook.com 
    > Created Time    : 2018年11月11日 星期日 18时27分09秒
 ************************************************************************/
#include<stdio.h>
#include <signal.h>
#include <time.h>
    #include <unistd.h> 
    

void  function_timer()
{
    time_t t;
    char p[32];
    time(&t);
    //strftime(p, sizeof(p), "%T", localtime(&t));
    strftime(p, sizeof(p), "%Y-%m-%d %H:%M:%S", localtime(&t));

    printf("@Jalyn debug -----> date: %s \n", p);
}


int main(int argc,char *argv[])
{
    int ret;
    timer_t timer;
    struct sigevent evp;
    struct timespec spec;
    struct itimerspec time_value;

    evp.sigev_value.sival_ptr = &timer;
    /*定时器到期时，会产生一个信号*/
    evp.sigev_notify = SIGEV_SIGNAL; 
    evp.sigev_signo = SIGUSR1;
    signal(SIGUSR1, function_timer);

    /*时钟源选CLOCK_MONOTONIC主要是考虑到系统的实时时钟可能会在
    程序运行过程中更改，所以存在一定的不确定性，而CLOCK_MONOTONIC
    则不会，较为稳定*/
    ret = timer_create(CLOCK_MONOTONIC, &evp, &timer);
    if( ret )
        perror("timer_create");

    time_value.it_interval.tv_sec = 1;      /*每秒触发一次*/
    time_value.it_interval.tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &spec);         
    time_value.it_value.tv_sec = spec.tv_sec + 5;      /*5秒后启动*/
    time_value.it_value.tv_nsec = spec.tv_nsec + 0;

    ret = timer_settime(timer, CLOCK_MONOTONIC, &time_value, NULL);
    if( ret )
            perror("timer_settime");

    while(1)
    {
        printf("@Jalyn debug -----> main loop \n");
        sleep(1);
    }

}
