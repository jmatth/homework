#define ITERS 10000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

int main(int argc, char const* argv[])
{
    int i, j, b;
    int status;
    int fd[2];
    double sum;
    double diff;
    double avg;
    char *arr;
    pid_t pid;
    struct timeval start, end;

    for (b = 0; b < 11; ++b)
    {
        arr = malloc(1048576 * b);
        for (j = 0; j < 1048576 * b; ++j)
        {
            arr[j] = 1;
        }
        sum = 0;
        avg = 0;
        for (i = 0; i < 10000; i++) {
            pipe(fd);
            gettimeofday(&start, NULL);
            pid = fork();
            gettimeofday(&end, NULL);

            if (pid != 0) {
                waitpid(pid, &status, 0);
                close(fd[1]);
                read(fd[0], &diff, sizeof(diff));
                close(fd[0]);
                if (diff <= 0)
                {
                    --i;
                    continue;
                }
                sum += diff;
            } else {
                close(fd[0]);
                diff = end.tv_usec - start.tv_usec;
                write(fd[1], &diff, sizeof(diff));
                close(fd[1]);
                free(arr);
                exit(0);
            }
        }
        avg = (double)sum / 10000;
        printf("avg time to fork with %dMB allocated: %f\n", (int)pow(2, (double)b), avg);
        free(arr);
    }

    return 0;
}
