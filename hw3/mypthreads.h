#include <ucontext.h>

#ifndef MYPTHREAD_H
#define MYPTHREAD_H 1

#define STACKSIZE 16384

enum _mypthread_state {
    RUNNING,
    SLEEPING,
    RUNNABLE,
    ZOMBIE,
    DEAD
};

typedef enum _mypthread_state mypthread_state;

struct mypthread_cont {
    int tid;
    mypthread_state state;
    ucontext_t context;
    char stack[STACKSIZE];
    void *retval;
    int sleeping_on_tid;
    long int sleeping_on_mutex;
    short int in_mypthreads;
};

struct mypthread_mutex {
    short int locked;
    long int id;
};

/**********************
 *  Internal helpers  *
 **********************/


typedef struct mypthread_cont mypthread_cont_t;

typedef struct mypthread_mutex mypthread_mutex_t;

typedef int mypthread_mutexattr_t;

typedef int mypthread_t;

void sched();

void init_timer();

inline void init_main_thread();

inline void init_thread(mypthread_cont_t*, int);


/*****************
 *  Cooperative  *
 *****************/

void mypthread_create(mypthread_t *, const pthread_attr_t *, void* (*)(void*), void*);

void mypthread_yield();

int mypthread_join(mypthread_t, void**);

void mypthread_exit();

/*********************
 *  Synchronization  *
 *********************/
void mypthread_mutex_init(mypthread_mutex_t *mutex, mypthread_mutexattr_t *attr);

int mypthread_mutex_lock(mypthread_mutex_t*);

int mypthread_mutex_unlock(mypthread_mutex_t*);

#endif
