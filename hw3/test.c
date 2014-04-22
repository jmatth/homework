#include "mypthreads.h"
#include <stdlib.h>
#include <stdio.h>

void* secondt(void *unused)
{
    int i;

    printf("   In second thread\n");
    for(i = 0; i < 1000000; i++)
    {
        printf("------->%d in 2nd thread\n", i);
    }
    mypthread_exit(0xdeadbeef);
    printf("   ERROR: this should never print\n");

    return NULL;
}

int main(int argc, char *argv[])
{
    int i;
    mypthread_t second_thread;
    void *retval = NULL;

    printf("Starting main\n");

    printf("Starting second thread\n");
    mypthread_create(&second_thread, NULL, &secondt, NULL);

    for(i = 0; i < 10000; i++)
    {
        printf("%d in main thread\n", i);
    }

    mypthread_join(second_thread, &retval);
    printf("Got 0x%x back from second thread\n", retval);

    return 0;
}
