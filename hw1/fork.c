#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

int main(int argc, char const* argv[])
{
    int i, j, bsize;
    int status;
    int fd[2];
    double avg;
    char *arr;
    pid_t pid;
    struct timeval sum, start, end, childEnd, timeDiff;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: fork <size>\n");
        exit(1);
    }

    bsize = atoi(argv[1]);

    arr = malloc(1048576 * bsize);
    for (j = 0; j < 1048576 * bsize; ++j)
    {
        arr[j] = 1;
    }
    sum.tv_sec = 0;
    sum.tv_usec = 0;
    for (i = 0; i < 10000; i++) {
        pipe(fd);
        gettimeofday(&start, NULL);
        pid = fork();
        gettimeofday(&end, NULL);

        if (pid > 0) {
            waitpid(pid, &status, 0);
            close(fd[1]);
            read(fd[0], &childEnd, sizeof(struct timeval));
            close(fd[0]);
            if (timercmp(&end, &childEnd, <))
            {
                timersub(&end, &start, &timeDiff);
            }
            else
            {
                timersub(&childEnd, &start, &timeDiff);
            }
            timeradd(&sum, &timeDiff, &sum);
        } else if(pid == 0) {
            arr[3] = 3;
            close(fd[0]);
            write(fd[1], &end, sizeof(struct timeval));
            close(fd[1]);
            free(arr);
            exit(0);
        } else{
            fprintf(stderr, "Problem with fork()! I'll just die now. X_X\n");
            exit(1);
        }
    }
    avg = ((double)sum.tv_sec * 1000000 + (double)sum.tv_usec) / 10000;
    printf("avg time to fork with %dMB allocated: %f\n", bsize, avg);
    free(arr);

    return 0;
}
