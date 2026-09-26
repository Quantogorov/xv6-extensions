#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#define EXIT_FAILURE 1
char buf;


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
	return buf;
}

void playPong(int isChild,int rec[], int ans[])
{	
	// parent gets the serve
	if(isChild)
	{ 
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
			buf = 0x46;
		}
		shoot(ans);
	}
	
}




int main(int argc, char *argv[])
{
	int pid;
	int PtoC[2];
	int CtoP[2];
	int isChild;
	buf = 0x00;
	int numBytes = 1;
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
			playPong(isChild,CtoP, PtoC);

	}

		
	
	return 0;
}

