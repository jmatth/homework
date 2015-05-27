#define MB 1048576
#define ITERS 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const* argv[])
{
    int i, j, bsize;
    int ptc[2];
    int ctp[2];
    double avg;
    char *arr;
    pid_t pid;
    struct timeval sum, start, end, timeDiff;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: pipe <size>\n");
        exit(1);
    }

    bsize = atoi(argv[1]);

    sum.tv_sec = 0;
    sum.tv_usec = 0;
    pipe(ptc);
    pipe(ctp);
    pid = fork();
    if (pid > 0)
    {
        close(ptc[0]);
        close(ctp[1]);

        arr = malloc(MB * bsize);
        if (arr == NULL)
        {
            fprintf(stderr, "malloc returned null. X_X\n");
            exit(1);
        }

        for (j = 0; j < MB * bsize; ++j)
        {
            arr[j] = 1;
        }

        for (i = 0; i < ITERS; ++i)
        {
            gettimeofday(&start, NULL);
            write(ptc[1], arr, MB * bsize);
            read(ctp[0], arr, MB * bsize);
            gettimeofday(&end, NULL);

            timersub(&end, &start, &timeDiff);
            timeradd(&sum, &timeDiff, &sum);
        }

        close(ctp[0]);
        close(ptc[1]);
    }
    else if (pid == 0)
    {
        close(ptc[1]);
        close(ctp[0]);

        arr = malloc(MB * bsize);

        if (arr == NULL)
        {
            fprintf(stderr, "malloc returned null. X_X\n");
            exit(1);
        }

        for (i = 0; i < ITERS; ++i)
        {
            read(ptc[0], arr, MB * bsize);
            write(ctp[1], arr, MB * bsize);
        }
        free(arr);
        exit(0);
    }
    else {
        fprintf(stderr, "Problem with fork()! I'll just die now. X_X\n");
        exit(1);
    }

    avg = ((double)sum.tv_sec * 1000000 + (double)sum.tv_usec) / ITERS;
    printf("%d\t%f\n", bsize, avg);
    free(arr);

    return 0;
}
