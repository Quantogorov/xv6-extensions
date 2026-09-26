#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

char buf;
unsigned int num_exchanges;

// sends the byte 0x01 to the other side
void shoot(int pipe[]) 
{
	if(write(pipe[1], &buf, 1) == -1) {
		printf("\nwrite error!");
		exit(EXIT_FAILURE);
	}
	return;
}

//reads in the value from the pipe into the buffer and returns the char inside
char receive(int pipe[])
{
	if(read(pipe[0], &buf, 1) == -1) {
		printf("\nread error!");
		exit(EXIT_FAILURE);
	}
	if(buf == 0x01) { // signal for child to exit
		exit(EXIT_FAILURE);
	}
	return buf;
}

unsigned int playPong(int isChild,int rec[], int ans[])
{	
	unsigned int time;

	// parent gets the serve
	if(isChild==0)
	{ 
		starttime();
		buf = 0x46;
		shoot(ans);		
	}
	while (1)
	{	
		receive(rec);
		printf("\n%c",buf);
		if(isChild) {
			buf = 0x73;
		} else {
			num_exchanges += 1;
			buf = 0x46;
		}
		if(num_exchanges == 10000) {
			buf = 0x01;
			shoot(ans);
			time = endtime();
			break;
		}
		shoot(ans);
	}
	return time;
	
}




int main(int argc, char *argv[])
{
	int pid;
	int PtoC[2];
	int CtoP[2];
	int isChild;
	buf = 0x00;
	int numBytes = 1;
	unsigned int time;
	// initialize both pipes and check for errors
	
	if(pipe(PtoC) == -1 || pipe(CtoP) == -1)
	{
		printf("\npipe error");
		exit(EXIT_FAILURE);
	}
	// initialize values of the pipes to 0
	write(PtoC[1], &buf, numBytes);
	write(CtoP[1], &buf, numBytes);
	pid = fork();
	switch(pid) 
	{
		case -1:
			printf("\nfork error!");
			exit(EXIT_FAILURE);
		case 0:
			printf("\nChild spawned");
			isChild = 1;
			playPong(isChild, PtoC, CtoP);
		default:
			isChild = 0;
			num_exchanges = 0;
			time = playPong(isChild,CtoP, PtoC);

	}
	printf("total of %d exchanges within %d ms, which is %d ex/sec!\n", num_exchanges, time,(num_exchanges * 1000 )/time);
	return 0;
}

