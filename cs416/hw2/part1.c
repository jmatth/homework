#define ITERS 10000
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

void catchDivByZero()
{
    unsigned char marker;
    static int counter = 0;
    counter++;
    if (counter > ITERS)
    {
        // Magic numbers divined from gdb
        *(int*)(&marker + 69) += 8;
    }
}

int main(int argc, char *argv[])
{
    signal(SIGFPE, catchDivByZero);
    int i;
    int dummy;
    struct timeval start, end, timeDiff;

    timeDiff.tv_sec = 0;
    timeDiff.tv_usec = 0;

    gettimeofday(&start, NULL);
    dummy = 1 / 0;
    gettimeofday(&end, NULL);

    timersub(&end, &start, &timeDiff);
    printf("%d %ld\n", ITERS, timeDiff.tv_sec * 1000000 + timeDiff.tv_usec);

    return 0;
}
