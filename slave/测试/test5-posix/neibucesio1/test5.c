#include <signal.h>    /* union sigval / struct sigevent */
#include <stdio.h>    /* printf */
#include <string.h>    /* memset */
#include <unistd.h> /* sleep */
#include <time.h>

#define printf_with_time(format, ...)                                        \
{                                                                            \
    struct timespec spec;                                                    \
    clock_gettime(CLOCK_MONOTONIC, &spec);                                    \
    printf("[%lu:%lu]"format"\n", spec.tv_sec, spec.tv_nsec, ##__VA_ARGS__);\
}


void timer_notify_cb(union sigval val)
{
    printf_with_time("timer expiration");

}

int main(void)
{
    /* Variable Definition */
    timer_t id;
    struct timespec spec;
    struct sigevent ent;
    struct itimerspec value;
    struct itimerspec get_val;

    /* Init */
    memset(&ent, 0x00, sizeof(struct sigevent));
    memset(&get_val, 0x00, sizeof(struct itimerspec));
    
    /* create a timer */
    ent.sigev_notify = SIGEV_THREAD;
    ent.sigev_notify_function = timer_notify_cb;
    printf_with_time("create timer");
    timer_create(CLOCK_REALTIME, &ent, &id);        /* CLOCK_REALTIME */

    /* start a timer */
    clock_gettime(CLOCK_REALTIME, &spec);            /* CLOCK_REALTIME */
    value.it_value.tv_sec = spec.tv_sec + 2;
    value.it_value.tv_nsec = spec.tv_nsec + 0;
    value.it_interval.tv_sec = 1;    /* per second */
    value.it_interval.tv_nsec = 0;
    printf_with_time("start timer");
    timer_settime(id, TIMER_ABSTIME, &value, NULL); /* TIMER_ABSTIME */

    sleep(10);
    printf_with_time("delete timer");
    timer_delete(id);
    return 0;
}

