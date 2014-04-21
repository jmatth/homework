#include "mypthreads.h"
#include <stdlib.h>
#include <stdio.h>

void* secondt(void *unused)
{
    printf("   In second thread\n");
    printf("   Yielding\n");
    mypthread_yield();
    printf("   Back in second thread after yield\n");
    mypthread_exit((void*)0xdeadbeef);
    printf("   ERROR: this should never print\n");

    return NULL;
}

int main(int argc, char *argv[])
{
    mypthread_t second_thread;
    void *retval = NULL;

    printf("Starting main\n");

    printf("Starting second thread\n");
    mypthread_create(&second_thread, NULL, &secondt, NULL);
    printf("thread address: 0x%x\n", second_thread);
    printf("Past second thread\n");
    mypthread_yield();
    printf("Trying to get ret from dead second thread\n");
    mypthread_join(second_thread, &retval);
    printf("Got 0x%x back from second thread\n", retval);

    return 0;
}
