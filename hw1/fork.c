#define ITERS 10000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const* argv[])
{
    int i;
    int status;
    int fd[2];
    long int sum = 0;
    long int diff;
    double avg;
    pid_t pid;
    struct timeval start, end;

    for (i = 0; i < 10000; i++) {
        pipe(fd);
        gettimeofday(&start, NULL);
        pid = fork();
        gettimeofday(&end, NULL);

        if (pid != 0) {
            waitpid(pid, &status, 0);
            close(fd[1]);
            read(fd[0], &diff, sizeof(diff));
            sum += diff;
        } else {
            close(fd[0]);
            diff = end.tv_usec - start.tv_usec;
            write(fd[1], &diff, sizeof(diff));
            exit(0);
        }
    }

    avg = (double)sum / 10000;
    printf("avg time to fork: %f\n", avg);

    return 0;
}
