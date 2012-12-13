#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define SOME_LARGE_NUMBER (10000)
#define PAGE_SIZE (8192)
#define ARRAY_SIZE (1024*4096)
#define MEGABYTE (1048576)

int main()
{

	//timer
	struct timeval start;
	struct timeval end;
	int timeTaken;

	register int *a=sbrk(sizeof(int)*ARRAY_SIZE+PAGE_SIZE);

	register int i = 0, SOME_ACCESS_SIZE=0, times = 0, track = 0, lasti = 0, outerCount=0;

	int dummy;

	for (SOME_ACCESS_SIZE = 1;
			SOME_ACCESS_SIZE <= MEGABYTE*8;
			SOME_ACCESS_SIZE = SOME_ACCESS_SIZE*2)
	{

		gettimeofday(&start,NULL);

		for (times=0; times<SOME_LARGE_NUMBER; times++)
		{
			//previous program determined line size to be 64
			//since i is an int 4*16=64.
			for (i = 0; i < SOME_ACCESS_SIZE; i += 16)
			{
				dummy=a[i];
			}
		}

		gettimeofday(&end,NULL);

		timeTaken = (end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec *1000000 + start.tv_usec);


		printf("%d\t%ld\n", SOME_ACCESS_SIZE*4, timeTaken);
	}

	return 0;
}
