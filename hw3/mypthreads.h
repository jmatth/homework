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
};

typedef struct mypthread_cont mypthread_cont_t;

typedef int mypthread_t;

void mypthread_create(mypthread_t *, const pthread_attr_t *, void* (*)(void*), void*);

void mypthread_yield();

int mypthread_join(mypthread_t, void**);

void mypthread_exit();

#endif
