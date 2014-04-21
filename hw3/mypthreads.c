#include <stdlib.h>
#include <ucontext.h>
#include "mypthreads.h"
#include <stdio.h>

int thread_table_l;
mypthread_t **thread_table;
int curr_thread;
int last_scheduled;

void mypthread_create(mypthread_t *thread, const pthread_attr_t *attr,
                     void* (*func)(void*), void *arg)
{
    int i;
    const char firstcall = 1;
    mypthread_t *this_thread;

    if (firstcall)
    {
        /* fprintf(stderr, "---first call, making main thread---\n"); */
        thread_table = calloc(sizeof(mypthread_t*), 256);
        thread_table_l = 256;
        curr_thread = 0;
        // FIXME: How to make sure this is freed? Will it matter?
        thread_table[curr_thread] = malloc(sizeof(mypthread_t));
        getcontext(&(thread_table[0]->context));

        thread_table[curr_thread]->context.uc_stack.ss_sp = malloc(STACKSIZE);
        thread_table[curr_thread]->context.uc_stack.ss_size = STACKSIZE;
        thread_table[curr_thread]->context.uc_link = NULL;

        thread_table[curr_thread]->state = RUNNING;
    }

    this_thread = thread_table[curr_thread];

    for (i = 0; i < thread_table_l; ++i)
        if (thread_table[i] == NULL)
            break;

    /* fprintf(stderr, "Found open thread at %d\n", i); */

    curr_thread = i;
    thread_table[curr_thread] = thread;
    getcontext(&(thread->context));
    thread->context.uc_stack.ss_sp = malloc(STACKSIZE);
    thread->context.uc_stack.ss_size = STACKSIZE;
    thread->context.uc_link = &(this_thread->context);

    makecontext(&(thread->context), (void*)func, 1, arg);
    thread_table[curr_thread]->state = RUNNABLE;
    thread_table[curr_thread]->state = RUNNING;
    swapcontext(&(this_thread->context), &(thread->context));
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
