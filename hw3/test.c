#include "mypthreads.h"
#include <stdlib.h>
#include <stdio.h>

void* secondt(void *unused)
{
    printf("   In second thread\n");
    printf("   Yielding\n");
    mypthread_yield();
    printf("   Back in second thread after yield");
    return NULL;
    /* mypthread_yield(); */
}

int main(int argc, char *argv[])
{
    mypthread_t *second_thread;

    printf("Starting main\n");

    second_thread = malloc(sizeof(mypthread_t));

    printf("Starting second thread\n");
    mypthread_create(second_thread, NULL, &secondt, NULL);
    printf("Past second thread\n");
    mypthread_yield();
    printf("Back in main for last time\n");

    return 0;
}
