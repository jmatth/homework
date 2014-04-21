#include <ucontext.h>

#ifndef MYPTHREAD_H
#define MYPTHREAD_H 1

enum _mypthread_state {
    RUNNING,
    SLEEPING,
    RUNNABLE
};

typedef enum _mypthread_state mypthread_state;

struct mypthread {
    mypthread_state state;
    ucontext_t context;
};

typedef struct mypthread mypthread_t;

void mypthread_create(mypthread_t *, const pthread_attr_t *, void* (*)(void*));

void mypthread_yield();

#endif
