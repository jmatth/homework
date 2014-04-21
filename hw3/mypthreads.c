#include <stdlib.h>
#include <ucontext.h>
#include "mypthreads.h"

int thread_table_l;
mypthread_t **thread_table;
int curr_thread;
int last_scheduled;

void mysched()
{
}

void myphread_create(mypthread_t *thread, const pthread_attr_t *attr,
                     void* (*func)(void*), void *arg)
{
    int i;
    const char firstcall = 1;

    if (firstcall)
    {
        thread_table = calloc(sizeof(mypthread_t*), 256);
        thread_table_l = 256;
        curr_thread = 0;
        thread_table[curr_thread]->state = RUNNING;
        thread_table[0] = malloc(sizeof(mypthread_t));
        getcontext(&(thread_table[0]->context));
    }

    for (i = 0; i < thread_table_l; ++i)
    {
        if (thread_table[0] != NULL)
            break;
    }

    thread_table[i] = thread;
    makecontext(&(thread->context), (void*)func, 1, arg);
    thread_table[curr_thread]->state = RUNNABLE;
    thread_table[i]->state = RUNNING;
    swapcontext(&(thread_table[curr_thread]->context), &(thread->context));
}

void  mypthread_yield()
{
    mypthread_t *this_thread = thread_table[curr_thread];
    if (thread_table[++last_scheduled] == NULL)
        last_scheduled = 0;

    while(thread_table[last_scheduled] == NULL)
        ++last_scheduled;

    mypthread_t *next_thread = thread_table[last_scheduled];

    this_thread->state = RUNNABLE;
    next_thread->state = RUNNING;
    swapcontext(&(this_thread->context), &(next_thread->context));
}


