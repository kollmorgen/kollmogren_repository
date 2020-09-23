
    #include <stdio.h>
    #include <time.h>
    #include <sys/time.h>
    #include <stdlib.h>
    #include <signal.h>
     
    static int count = 0;
    struct itimerval timenow,time3;
int k=0;

    void set_timer()
    {
     	timenow.it_interval.tv_sec = 0;
    	timenow.it_interval.tv_usec = 1*1000;
    	timenow.it_value.tv_sec = 0;
    	timenow.it_value.tv_usec = 1000*1000;
    	setitimer(ITIMER_REAL, &timenow, NULL);
    }

    void set_timer3()
    {
     	time3.it_interval.tv_sec = 0;
    	time3.it_interval.tv_usec = 1*1000;
    	time3.it_value.tv_sec = 1;
    	time3.it_value.tv_usec = 0;
    	setitimer(ITIMER_REAL, &time3, NULL);
    }
     
    void signal_handler()
    {
    	
k++;
if(k==1000){count ++;
    	printf("time:%ds\n", count);k=0;}
    }
     
    int main()
    {
    	signal(SIGALRM, signal_handler);
    	
         set_timer3();
    	while(count < 500000000);
    	//printf("hello\n");
        exit(0);
    	return 1;
    }

