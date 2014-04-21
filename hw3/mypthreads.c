#include <stdlib.h>
#include <ucontext.h>
#include "mypthreads.h"
#include <stdio.h>

int thread_table_l;
mypthread_cont_t **thread_table;
int curr_thread;

void swtch(mypthread_t next, mypthread_state state)
{
    mypthread_cont_t *this_thread = thread_table[curr_thread];

    this_thread->state = state;
    thread_table[next]->state = RUNNING;
    curr_thread = next;

    swapcontext(&(this_thread->context), &(thread_table[next]->context));
}

void sched(mypthread_state state)
{
    int i, next_tid;

    /* FIXME: What if we don't find one? */
    for (i = 0; i < thread_table_l; ++i)
        if (thread_table[i] != NULL && thread_table[i]->state == RUNNABLE)
        {
            next_tid = i;
            break;
        }

    swtch(next_tid, state);
}

void mypthread_create(mypthread_t *thread, const pthread_attr_t *attr,
                     void* (*func)(void*), void *arg)
{
    int i, next_tid;
    const char firstcall = 1;
    mypthread_cont_t *this_thread;

    if (firstcall)
    {
        /* fprintf(stderr, "---first call, making main thread---\n"); */
        thread_table = calloc(sizeof(mypthread_cont_t*), 256);
        thread_table_l = 256;
        curr_thread = 0;
        // FIXME: How to make sure this is freed? Will it matter?
        thread_table[curr_thread] = malloc(sizeof(mypthread_cont_t));
        thread_table[curr_thread]->tid = curr_thread;
        thread_table[curr_thread]->sleeping_on_tid = -1;

        getcontext(&(thread_table[0]->context));

        thread_table[curr_thread]->context.uc_stack.ss_sp = thread_table[curr_thread]->stack;
        thread_table[curr_thread]->context.uc_stack.ss_size = STACKSIZE;
        thread_table[curr_thread]->context.uc_link = NULL;

        thread_table[curr_thread]->state = RUNNING;
    }

    this_thread = thread_table[curr_thread];

    for (i = 0; i < thread_table_l; ++i)
    {
        if (thread_table[i] == NULL)
        {
            next_tid = i;
            thread_table[next_tid] = malloc(sizeof(mypthread_cont_t));
            thread_table[next_tid]->tid = next_tid;
            thread_table[next_tid]->sleeping_on_tid = -1;
            break;
        }
        else if (thread_table[i]->state == DEAD)
        {
            next_tid = i;
            break;
        }
    }

    /* fprintf(stderr, "Found open thread at %d\n", i); */

    *thread = next_tid;

    getcontext(&(thread_table[next_tid]->context));
    thread_table[next_tid]->context.uc_stack.ss_sp = thread_table[next_tid]->stack;
    thread_table[next_tid]->context.uc_stack.ss_size = STACKSIZE;
    thread_table[next_tid]->context.uc_link = &(this_thread->context);

    makecontext(&(thread_table[next_tid]->context), (void*)func, 1, arg);

    swtch(next_tid, RUNNABLE);

    /* thread_table[curr_thread]->state = RUNNABLE; */
    /* thread_table[curr_thread]->state = RUNNING; */
    /* swapcontext(&(this_thread->context), &(thread->context)); */
}

void  mypthread_yield()
{
    sched(RUNNABLE);
}

void mypthread_exit(void *retval)
{
    int i;
    thread_table[curr_thread]->retval = retval;

    for (i = 0; i < thread_table_l; ++i)
    {
        if (thread_table[i] != NULL &&
            thread_table[i]->sleeping_on_tid == thread_table[curr_thread]->tid)
        {
            swtch(i, DEAD);
        }
    }

    sched(ZOMBIE);
}

int mypthread_join(mypthread_t thread, void **retval)
{
    mypthread_cont_t *this_thread;
    mypthread_cont_t *real_thread = thread_table[thread];

    if (real_thread->state == ZOMBIE)
    {
        real_thread->state = DEAD;
        if (retval != NULL)
            *retval = real_thread->retval;
        return 0;
    }

    this_thread = thread_table[curr_thread];
    this_thread->sleeping_on_tid = real_thread->tid;

    sched(SLEEPING);

    real_thread->state = DEAD;
    if (retval != NULL)
        *retval = real_thread->retval;

    return 0;
}
