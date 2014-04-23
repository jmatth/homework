#include "mypthread.h"
#include <stdlib.h>
#include <stdio.h>

mypthread_mutex_t *mutex;

void* secondt(void *unused)
{
    printf("   In second thread\n");
    printf("   Trying to lock mutex in second thread\n");
    mypthread_mutex_lock(mutex);
    printf("   Got the mutex in second thread\n");

    mypthread_exit(0xdeadbeef);
    printf("   ERROR: this should never print\n");

    return NULL;
}

int main(int argc, char *argv[])
{
    mypthread_t second_thread;
    void *retval = NULL;

    printf("Starting main\n");

    printf("Malloc-ing mutex\n");
    mutex = malloc(sizeof(mypthread_mutex_t));
    printf("Initializing mutex\n");
    mypthread_mutex_init(mutex, NULL);

    printf("Locking mutex in main\n");
    mypthread_mutex_lock(mutex);

    printf("Starting second thread\n");
    mypthread_create(&second_thread, NULL, &secondt, NULL);

    printf("Unlocking mutex in main thread\n");
    mypthread_mutex_unlock(mutex);

    mypthread_join(second_thread, &retval);
    printf("Got 0x%x back from second thread\n", retval);

    return 0;
}
