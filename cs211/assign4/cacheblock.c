#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#define SOME_LARGE_NUMBER (100000)
#define PAGE_SIZE (8192)
#define ARRAY_SIZE (1024*4096)

int main()
{

	//timer
	struct timeval start;
	struct timeval end;
	int timeTaken;

	register int *a=sbrk(sizeof(int)*ARRAY_SIZE+PAGE_SIZE);

	register int i = 0, SOME_ACCESS_SIZE=0, times = 0, track = 0, lasti = 0, outerCount=0;

	int dummy;

	for (SOME_ACCESS_SIZE = 128;
			SOME_ACCESS_SIZE >= 4;
			SOME_ACCESS_SIZE=SOME_ACCESS_SIZE/2)
	{
		gettimeofday(&start,NULL);

		for(outerCount=0, i=lasti;
				outerCount<=SOME_LARGE_NUMBER;
				outerCount++)
		{

			for (; i <= 8192*SOME_ACCESS_SIZE
					+8192*SOME_ACCESS_SIZE*track
					+lasti;
					i += SOME_ACCESS_SIZE)
			{
				dummy=a[i];

			}
		}

		gettimeofday(&end,NULL);

		track++;
		lasti = i;

		timeTaken = (end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec *1000000 + start.tv_usec);
		printf("%d\t%ld\n",
				SOME_ACCESS_SIZE*4, timeTaken);
	}

	return 0;
}
