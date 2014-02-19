#define KB 1024
#define ITERS 10000
#define PAGE_SIZE (8192)
#define MB (1048576)
#define ARRAY_SIZE (4096)
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
    register int *arr;
    register int dummy;
    int bsize;
    double avg;
    struct timeval start, end, sum, diff;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: cacheLine <size>\n");
        exit(1);
    }

    bsize = atoi(argv[1]);
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
        for (j = 0; j < ARRAY_SIZE; j += bsize)
        {
            dummy = arr[j];
        }
    }
    gettimeofday(&end, NULL);
    timersub(&end, &start, &diff);
    timeradd(&sum, &diff, &sum);


    /* avg = ((double)sum.tv_sec * 1000000 + (double)sum.tv_usec) / ITERS; */
    avg = ((double)sum.tv_sec * 1000000 + (double)sum.tv_usec) / (ARRAY_SIZE / (bsize));
    printf("%d\t%f\n", bsize, avg);

    return 0;
}
