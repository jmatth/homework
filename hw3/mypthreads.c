#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include "mypthreads.h"
#include "mypthread_queue.h"

int thread_table_l;
mypthread_cont_t **thread_table;
int curr_thread;

queue threadqueue;

struct itimerval timer;

#define LOCKLIB thread_table[curr_thread]->in_mypthreads = 1
#define UNLOCKLIB thread_table[curr_thread]->in_mypthreads = 0

void timer_handler(int sig)
{
    if (!thread_table[curr_thread]->in_mypthreads)
    {
        LOCKLIB;
        sched(RUNNABLE);
    }

    UNLOCKLIB;
}

inline void swtch(mypthread_t next, mypthread_state state)
{
    mypthread_cont_t *this_thread = thread_table[curr_thread];

    if (state == RUNNABLE)
        myenqueue(curr_thread);

    this_thread->state = state;
    thread_table[next]->state = RUNNING;
    curr_thread = next;

    swapcontext(&(this_thread->context), &(thread_table[next]->context));
}

inline void sched(mypthread_state state)
{
    int i, next_tid;

    next_tid = mydequeue();

    if(next_tid == -1)
        return;

    swtch(next_tid, state);
}

inline void init_thread(mypthread_cont_t *thread, int tid)
{
    getcontext(&(thread->context));
    thread->tid = tid;
    thread->sleeping_on_tid = -1;
    thread->context.uc_stack.ss_sp = thread->stack;
    thread->context.uc_stack.ss_size = STACKSIZE;
    thread->context.uc_link = &(thread_table[curr_thread]->context);
    thread->state = RUNNABLE;
    thread->in_mypthreads = 0;
}

void init_timer()
{
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1;

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void mypthread_create(mypthread_t *thread, const pthread_attr_t *attr,
                     void* (*func)(void*), void *arg)
{
    int i, next_tid;
    short int firstcall = 1;

    if (firstcall)
    {
        thread_table = calloc(sizeof(mypthread_cont_t*), 256);
        thread_table_l = 256;
        curr_thread = 0;
        // FIXME: How to make sure this is freed? Will it matter?
        thread_table[curr_thread] = malloc(sizeof(mypthread_cont_t));

        init_thread(thread_table[curr_thread], curr_thread);

        LOCKLIB;

        thread_table[curr_thread]->context.uc_link = NULL;
        thread_table[curr_thread]->state = RUNNING;

        myqueueinit();

        firstcall = 0;
        signal(SIGVTALRM, timer_handler);

        init_timer();
    }

    LOCKLIB;

    for (i = 0; i < thread_table_l; ++i)
    {
        if (thread_table[i] == NULL)
        {
            next_tid = i;
            thread_table[next_tid] = malloc(sizeof(mypthread_cont_t));
            break;
        }
        else if (thread_table[i]->state == DEAD)
        {
            next_tid = i;
            break;
        }
    }

    init_thread(thread_table[next_tid], next_tid);

    *thread = next_tid;

    makecontext(&(thread_table[next_tid]->context), (void*)func, 1, arg);

    swtch(next_tid, RUNNABLE);
    UNLOCKLIB;
}

void  mypthread_yield()
{
    LOCKLIB;
    sched(RUNNABLE);
    UNLOCKLIB;
}

void mypthread_exit(void *retval)
{
    LOCKLIB;
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
    UNLOCKLIB;
}

int mypthread_join(mypthread_t thread, void **retval)
{
    LOCKLIB;
    mypthread_cont_t *this_thread;
    mypthread_cont_t *real_thread = thread_table[thread];

    if (real_thread->state == ZOMBIE)
    {
        real_thread->state = DEAD;
        if (retval != NULL)
            *retval = real_thread->retval;
        UNLOCKLIB;
        return 0;
    }

    this_thread = thread_table[curr_thread];
    this_thread->sleeping_on_tid = real_thread->tid;

    sched(SLEEPING);

    real_thread->state = DEAD;
    if (retval != NULL)
        *retval = real_thread->retval;

    UNLOCKLIB;
    return 0;
}




void myqueueinit()
{
    threadqueue.front = NULL;
    threadqueue.end = NULL;
}

int myenqueue(mypthread_t thread)
{
    threadnode *node;

    node = malloc(sizeof(threadnode));
    node->tid = thread;
    node->next = NULL;

    if (threadqueue.end == NULL)
    {
        threadqueue.end = node;
        threadqueue.front = node;
    }
    else
    {
        threadqueue.end->next = node;
        threadqueue.end = node;
    }

    return 0;
}

mypthread_t mydequeue()
{
    mypthread_t ret;

    if (threadqueue.front == NULL)
        return -1;

    ret = threadqueue.front->tid;

    threadqueue.front = threadqueue.front->next;
    if (threadqueue.front == NULL)
        threadqueue.end = NULL;

    return ret;
}
