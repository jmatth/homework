#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *arr;
volatile short int sorted = 0;
pthread_t *threads;
pthread_mutex_t *mutexes;

void* swap(void *ptrindex)
{
    int tmp;
    int index = (int)ptrindex;

    int *left = &(arr[index]);
    int *right = &(arr[index + 1]);

    pthread_mutex_t *leftm = &mutexes[index];
    pthread_mutex_t *rightm = &mutexes[index+1];

    while(!sorted)
    {
        pthread_mutex_lock(leftm);
        pthread_mutex_lock(rightm);

        if (*left > *right)
        {
            tmp = *left;
            *left = *right;
            *right = tmp;
        }

        pthread_mutex_unlock(leftm);
        pthread_mutex_unlock(rightm);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int i;
    int nelems = argc - 1;
    arr = malloc(sizeof(int) * nelems);

    threads = malloc(sizeof(pthread_t) * (nelems - 1));
    mutexes = malloc(sizeof(pthread_mutex_t) * nelems);

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
        pthread_mutex_init(&(mutexes[i]), NULL);
    }

    for (i = 0; i < nelems - 1; ++i)
    {
        pthread_create(&(threads[i]), NULL, &swap, (void*)i);
    }

    i = 0;
    while (i < nelems - 1)
    {
        if (arr[i] > arr[i+1])
            // Start over if it's still not sorted.
            i = 0;
        else
            // Otherwise keep going.
            i++;
    }

    // Checked the whole array and it's good, set the sorted flag.
    sorted = 1;

    void* retval;
    for (i = 0; i < nelems - 1; ++i)
    {
        pthread_join((threads[i]), &retval);
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
