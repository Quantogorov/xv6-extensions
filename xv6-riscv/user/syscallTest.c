#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[])
{
	uint64 free_pages = memfree();
	printf("The number of free physical pages is: %lu\n", free_pages);
	return 0;
}
