// set timers to measure the time which has elapsed between function calls
#include "kernel/types.h" 
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

unsigned int t_count;

//start timer by fetching the current tick count
void starttime()
{
	t_count = uptime();
	return;
}

//calculates the total time elapsed in ms by fetching current ticks counted and 
unsigned int endtime()
{
	unsigned int ticks_elapsed;
	unsigned int time_elapsed;
	ticks_elapsed = uptime() - t_count;
	time_elapsed = 100 * ticks_elapsed;
	return time_elapsed;
}
	
