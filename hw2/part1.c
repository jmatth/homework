#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void catchDivByZero()
{
    unsigned char marker;
    // Magic numbers divined from gdb
    *(int*)(&marker + 69) += 8;
    return;
}

int main(int argc, char *argv[])
{
    signal(SIGFPE, catchDivByZero);
    int dummy;

    printf("before div by zero\n");
    dummy = 1 / 0;
    printf("past div by zero.\n");
    return 0;
}
