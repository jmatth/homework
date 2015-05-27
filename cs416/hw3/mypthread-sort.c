#include <stdio.h>
#include <stdlib.h>
#include "mypthread.h"

int *arr;
volatile short int sorted = 0;
mypthread_t *threads;
mypthread_mutex_t *mutexes;

void* swap(void *ptrindex)
{
    int tmp;
    int index = (int)ptrindex;

    int *left = &(arr[index]);
    int *right = &(arr[index + 1]);

    mypthread_mutex_t *leftm = &mutexes[index];
    mypthread_mutex_t *rightm = &mutexes[index+1];

    while(!sorted)
    {
        mypthread_mutex_lock(leftm);
        mypthread_mutex_lock(rightm);

        if (*left < *right)
        {
            tmp = *left;
            *left = *right;
            *right = tmp;
        }

        mypthread_mutex_unlock(rightm);
        mypthread_mutex_unlock(leftm);

        mypthread_yield();
    }

    mypthread_exit(NULL);

    return NULL;
}

int main(int argc, char *argv[])
{
    int i;
    int nelems = argc - 1;
    arr = malloc(sizeof(int) * nelems);

    threads = malloc(sizeof(mypthread_t) * (nelems - 1));
    mutexes = malloc(sizeof(mypthread_mutex_t) * nelems);

    if (argc <= 1)
    {
        return 0;
    } else if (argc == 2) {
        printf("%s\n", argv[1]);
        return 0;
    }

    for (i = 0; i < argc - 1; ++i)
    {
        arr[i] = atoi(argv[i + 1]);
        mypthread_mutex_init(&(mutexes[i]), NULL);
    }

    for (i = 0; i < nelems - 1; ++i)
    {
        mypthread_create(&(threads[i]), NULL, &swap, (void*)i);
    }

    i = 0;
    while (i < nelems - 1)
    {
        mypthread_mutex_lock(&mutexes[i]);
        mypthread_mutex_lock(&mutexes[i+1]);

        if (arr[i] < arr[i+1])
        {
            mypthread_mutex_unlock(&mutexes[i+1]);
            mypthread_mutex_unlock(&mutexes[i]);
            // Start over if it's still not sorted.
            i = 0;
        }
        else
        {
            mypthread_mutex_unlock(&mutexes[i+1]);
            mypthread_mutex_unlock(&mutexes[i]);
            i++;
        }
    }

    // Checked the whole array and it's good, set the sorted flag.
    sorted = 1;

    void* retval;
    for (i = 0; i < nelems - 1; ++i)
    {
        mypthread_join((threads[i]), &retval);
    }

    for (i = 0; i < nelems; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    free(threads);
    free(mutexes);

    return 0;
}
