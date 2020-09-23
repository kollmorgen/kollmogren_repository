#include <signal.h>    /* union sigval / struct sigevent */
#include <stdio.h>    /* printf */
#include <string.h>    /* memset */
#include <unistd.h> /* sleep */
#include <time.h>

struct sigevent sigev;
static struct timeval last_sig;
static timer_t timer;

#define TIMER_HANDLE INTEGER16
#define TIMEVAL unsigned long long


void timer_notify(sigval_t val)
{
	if(gettimeofday(&last_sig,NULL)) {
		perror("gettimeofday()");
	}
	TimeDispatch();
}


void TimeDispatch(void)
{
	TIMER_HANDLE i;
	TIMEVAL next_wakeup = TIMEVAL_MAX; 
	UNS32 overrun = (UNS32)getElapsedTime();

	TIMEVAL real_total_sleep_time = total_sleep_time + overrun;

	s_timer_entry *row;

	for(i=0, row = timers; i <= last_timer_raw; i++, row++)
	{
		if (row->state & TIMER_ARMED) /* if row is active */
		{
			if (row->val <= real_total_sleep_time) /* to be trigged */
			{
				if (!row->interval) /* if simply outdated */
				{
					row->state = TIMER_TRIG; /* ask for trig */
				}
				else /* or period have expired */
				{
					/* set val as interval, with 32 bit overrun correction, */
					/* modulo for 64 bit not available on all platforms     */
					row->val = row->interval - (overrun % (UNS32)row->interval);
					row->state = TIMER_TRIG_PERIOD; /* ask for trig, periodic */
					/* Check if this new timer value is the soonest */
					if(row->val < next_wakeup)
						next_wakeup = row->val;
				}
			}
			else
			{
				/* Each armed timer value in decremented. */
				row->val -= real_total_sleep_time;

				/* Check if this new timer value is the soonest */
				if(row->val < next_wakeup)
					next_wakeup = row->val;
			}
		}
	}

	/* Remember how much time we should sleep. */
	total_sleep_time = next_wakeup;

	/* Set timer to soonest occurence */
	setTimer(next_wakeup);

	/* Then trig them or not. */
	for(i=0, row = timers; i<=last_timer_raw; i++, row++)
	{
		if (row->state & TIMER_TRIG)
		{
			row->state &= ~TIMER_TRIG; /* reset trig state (will be free if not periodic) */
			if(row->callback)
				(*row->callback)(row->d, row->id); /* trig ! */
		}
	}
}


void main(void)
{

if(gettimeofday(&last_sig,NULL)){
   perror("gettimeofday()");
}


#if defined(__UCLIBC__)
	int ret;
	ret = timer_create(CLOCK_PROCESS_CPUTIME_ID, NULL, &timer);
	signal(SIGALRM, timer_notify);
#else
	memset (&sigev, 0, sizeof (struct sigevent));
	sigev.sigev_value.sival_int = 0;
	sigev.sigev_notify = SIGEV_THREAD;
	sigev.sigev_notify_attributes = NULL;
	sigev.sigev_notify_function = timer_notify;

	if(timer_create (CLOCK_REALTIME, &sigev, &timer)) {
		perror("timer_create()");
	}
#endif


}
