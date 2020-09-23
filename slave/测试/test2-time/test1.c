    #include <stdio.h> 
    #include <signal.h> 
    #include <time.h> 
    #include <string.h> 
    #include <stdlib.h> 
    #include <unistd.h> 
    
     int count=0;   
     int aa=0;  
    float time33;
    void timer_thread(union sigval v) 
    { 
        //printf("timer_thread function! %d\n", v.sival_int); 
        count ++;
        aa++;
        time33 = count *0.001;
        if(aa==1000){
         printf("time is %f\n",time33);aa=0;
}
          
        
    } 
     
    int main() 
    { 
        timer_t timerid; 
        struct sigevent evp; 
        memset(&evp, 0, sizeof(struct sigevent));       //清零初始化 
     
        evp.sigev_value.sival_int = 111;                //也是标识定时器的，回调函数可以获得 
        evp.sigev_notify = SIGEV_THREAD;                //线程通知的方式，派驻新线程 
        evp.sigev_notify_function = timer_thread;       //线程函数地址 
     
        if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1) 
        {   
            perror("fail to timer_create"); 
            exit(-1); 
        }   
     
        /* 第一次间隔it.it_value这么长,以后每次都是it.it_interval这么长,就是说it.it_value变0的时候会>装载it.it_interval的值 */
        struct itimerspec it; 
        it.it_interval.tv_sec = 0;  // 回调函数执行频率为1s运行1次
        it.it_interval.tv_nsec = 1*1000000; 
        it.it_value.tv_sec = 0;     // 倒计时3秒开始调用回调函数
        it.it_value.tv_nsec = 1*1000000; 
        count=0;
     aa=0;
        if (timer_settime(timerid, 0, &it, NULL) == -1) 
        {   
            perror("fail to timer_settime"); 
            exit(-1); 
        }   
     
        //pause();
        while (1);
     
        return 0; 
    } 
    /*
     * int timer_gettime(timer_t timerid, struct itimerspec *curr_value);
     * 获取timerid指定的定时器的值，填入curr_value
     */ 
