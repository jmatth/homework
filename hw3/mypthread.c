#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include "mypthread.h"

#define CLOCKTICKSEC  0
#define CLOCKTICKUSEC 10000

int thread_table_l;
mypthread_cont_t **thread_table;
int curr_thread;

long int next_mutex_id = 0;

threadqueue mainqueue;

struct itimerval timer;

short int firstcall = 1;

#define LOCKLIB thread_table[curr_thread]->in_mypthreads = 1
#define UNLOCKLIB thread_table[curr_thread]->in_mypthreads = 0

void exit_cleanup(){
    LOCKLIB;

    int i;

    if (firstcall)
        return;

    for (i = 0; i < thread_table_l; i++)
    {
        if (thread_table[i] == NULL)
            continue;

        free(thread_table[i]);
    }

    free(thread_table);

    myqueueempty(&mainqueue);
}

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
        myenqueue(curr_thread, &mainqueue);

    this_thread->state = state;
    thread_table[next]->state = RUNNING;
    curr_thread = next;

    swapcontext(&(this_thread->context), &(thread_table[next]->context));
}

inline void sched(mypthread_state state)
{
    int next_tid;

    next_tid = mydequeue(&mainqueue);

    if(next_tid == -1)
        return;

    swtch(next_tid, state);
}

inline void init_main_thread()
{
    thread_table = calloc(sizeof(mypthread_cont_t*), 256);
    thread_table_l = 256;
    curr_thread = 0;
    thread_table[curr_thread] = malloc(sizeof(mypthread_cont_t));

    init_thread(thread_table[curr_thread], curr_thread);

    LOCKLIB;

    thread_table[curr_thread]->context.uc_link = NULL;
    thread_table[curr_thread]->state = RUNNING;

    myqueueinit(&mainqueue);

    firstcall = 0;
    signal(SIGVTALRM, timer_handler);
    atexit(exit_cleanup);

    init_timer();
}

inline void init_thread(mypthread_cont_t *thread, int tid)
{
    getcontext(&(thread->context));
    thread->tid = tid;
    thread->context.uc_stack.ss_sp = thread->stack;
    thread->context.uc_stack.ss_size = STACKSIZE;
    thread->context.uc_link = &(thread_table[curr_thread]->context);
    thread->state = RUNNABLE;
    thread->in_mypthreads = 0;
    thread->sleeping_on = -1;
}

void init_timer()
{
    timer.it_interval.tv_sec = CLOCKTICKSEC;
    timer.it_interval.tv_usec = CLOCKTICKUSEC;

    timer.it_value.tv_sec = CLOCKTICKSEC;
    timer.it_value.tv_usec = CLOCKTICKUSEC;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

/**********************************
 *  Cooperative thread functions  *
 **********************************/

void mypthread_create(mypthread_t *thread, const pthread_attr_t *attr,
                     void* (*func)(void*), void *arg)
{
    int i, next_tid;

    if (firstcall)
        init_main_thread();

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

    myenqueue(next_tid, &mainqueue);
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
    thread_table[curr_thread]->retval = retval;

    if (thread_table[curr_thread]->sleeping_on != -1)
        myenqueue(thread_table[curr_thread]->sleeping_on, &mainqueue);

    sched(ZOMBIE);
    UNLOCKLIB;
}

int mypthread_join(mypthread_t thread, void **retval)
{
    LOCKLIB;
    mypthread_cont_t *real_thread = thread_table[thread];

    if (real_thread->state == ZOMBIE)
    {
        real_thread->state = DEAD;
        if (retval != NULL)
            *retval = real_thread->retval;
        UNLOCKLIB;
        return 0;
    }

    real_thread->sleeping_on = curr_thread;

    sched(SLEEPING);

    real_thread->state = DEAD;
    if (retval != NULL)
        *retval = real_thread->retval;

    UNLOCKLIB;
    return 0;
}


/*********************
 *  Synchronization  *
 *********************/
void mypthread_mutex_init(mypthread_mutex_t *mutex, mypthread_mutexattr_t *attr)
{
    if (firstcall)
        init_main_thread();

    LOCKLIB;

    mutex->locked = 0;
    mutex->id = next_mutex_id++;
    mutex->locked_by = -1;
    myqueueinit(&(mutex->sleeping_on));

    UNLOCKLIB;
}

int mypthread_mutex_lock(mypthread_mutex_t *mutex)
{
    LOCKLIB;

    while (mutex->locked)
    {
        myenqueue(curr_thread, &(mutex->sleeping_on));
        sched(SLEEPING);
    }

    mutex->locked = 1;
    mutex->locked_by = curr_thread;

    UNLOCKLIB;
    return 0;
}

int mypthread_mutex_unlock(mypthread_mutex_t *mutex)
{
    LOCKLIB;

    int next_tid;

    if (mutex->locked_by != curr_thread)
    {
        UNLOCKLIB;
        return 1;
    }

    mutex->locked = 0;
    mutex->locked_by = -1;

    next_tid = mydequeue(&(mutex->sleeping_on));

    if (next_tid != -1)
        swtch(next_tid, RUNNABLE);

    UNLOCKLIB;

    return 0;
}

int mypthread_mutex_trylock(mypthread_mutex_t *mutex)
{
    LOCKLIB;

    if (mutex->locked)
    {
        UNLOCKLIB;
        return EBUSY;
    }

    mutex->locked = 1;

    UNLOCKLIB;
    return 0;
}

int mypthread_mutex_destroy(mypthread_mutex_t *mutex)
{
    myqueueempty(&(mutex->sleeping_on));
    return 0;
}


/**************************
 *  Internal queue stuff  *
 **************************/

void myqueueinit(threadqueue *queue)
{
    queue->front = NULL;
    queue->end = NULL;
}

void myqueueempty(threadqueue *queue)
{
    while(mydequeue(queue) != -1);
}

int myenqueue(mypthread_t thread, threadqueue *queue)
{
    struct threadnode *node;

    node = malloc(sizeof(struct threadnode));
    node->tid = thread;
    node->next = NULL;

    if (queue->end == NULL)
    {
        queue->end = node;
        queue->front = node;
    }
    else
    {
        queue->end->next = node;
        queue->end = node;
    }

    return 0;
}

mypthread_t mydequeue(threadqueue *queue)
{
    mypthread_t ret;
    struct threadnode *curr_front;

    if (queue->front == NULL)
        return -1;

    curr_front = queue->front;

    ret = curr_front->tid;

    queue->front = curr_front->next;
    if (queue->front == NULL)
        queue->end = NULL;

    free(curr_front);

    return ret;
}
