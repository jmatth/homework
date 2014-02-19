#define ITERS 10000
#define MB 1048576
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const* argv[])
{
    int i, bsize;
    double avg;
    char *arr1;
    char *arr2;
    struct timeval sum, start, end, timeDiff;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: fork <size>\n");
        exit(1);
    }

    bsize = atoi(argv[1]);

    arr1 = malloc(MB * bsize);
    arr2 = malloc(MB * bsize);
    for (i = 0; i < MB * bsize; ++i)
    {
        arr1[i] = 1;
    }

    sum.tv_sec = 0;
    sum.tv_usec = 0;
    for (i = 0; i < ITERS; ++i)
    {
        gettimeofday(&start, NULL);
        memcpy(arr2, arr1, MB * bsize);
        gettimeofday(&end, NULL);
        timersub(&end, &start, &timeDiff);
        timeradd(&sum, &timeDiff, &sum);
    }

    avg = ((double)sum.tv_sec * 1000000 + (double)sum.tv_usec) / 10000;
    printf("%d\t%f\n", bsize, avg);
    free(arr1);
    free(arr2);

    return 0;
}
