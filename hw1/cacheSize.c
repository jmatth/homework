#define KB 1024
#define PAGE_SIZE (8192)
#define MB (1048576)
#define ARRAY_SIZE (1024*4096)
#define BIGNUMBER (10000)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    register int i;
    register int j = 0;
    register volatile int *arr;
    register int dummy;
    register int stepSize;
    register int testSize;
    double avg;
    struct timeval start, end, sum, diff;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: cacheSize <stepSize> <testSize>\n");
        exit(1);
    }

    stepSize = atoi(argv[1]);
    testSize = atoi(argv[2]) * KB;
    /* arr = malloc(sizeof(int) * ARRAY_SIZE + PAGE_SIZE); */
	arr = sbrk(sizeof(int)*ARRAY_SIZE+PAGE_SIZE);
    arr = (int *)((((int)arr>>13)<<13)+8192);

    sum.tv_sec = 0;
    sum.tv_usec = 0;

    for (i = 0; i < ARRAY_SIZE; ++i)
    {
        dummy = arr[i];
    }

    gettimeofday(&start, NULL);
    for (i = 0; i < BIGNUMBER; ++i)
    {
        for (j = 0; j < testSize; j += stepSize)
        {
            dummy = arr[j];
        }
    }
    gettimeofday(&end, NULL);
    timersub(&end, &start, &diff);
    timeradd(&sum, &diff, &sum);


    avg = ((double)sum.tv_sec * 1000000 + (double)sum.tv_usec) / (testSize / (stepSize));
    printf("%d\t%f\n", testSize / KB, avg);

    return 0;
}
