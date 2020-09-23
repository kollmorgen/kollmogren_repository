
    #include <stdio.h>
    #include <time.h>
    #include <sys/time.h>
    #include <stdlib.h>
    #include <signal.h>
   #include <time.h>
 # include <unistd.h>
     
    static long long count = 0;
float time2;
    static struct itimerval oldtv,itv;
     int ti=0;
    void set_timer()
    {
    	itv.it_interval.tv_sec = 0;
    	itv.it_interval.tv_usec = 5*1000;
    	itv.it_value.tv_sec = 0;
    	itv.it_value.tv_usec = 5*1000;
    	setitimer(ITIMER_REAL, &itv, NULL);
    }
     
    void signal_handler(int m)
    {
    	count ++;
        time2=count*0.005;
  ti++;
    if(ti==200){
         ti=0;
        printf("time:%f\n",time2);
     }
    }
     
    int main()
    {
    	//signal(SIGALRM, signal_handler);
    	//set_timer();

    	while(count < 10000){
usleep(1*1000)  ;
count ++;
        time2=count*0.001;
printf("time:%f\n",time2);
};
    	exit(0); //用于结束当前的进程
    	return 1;
    }
