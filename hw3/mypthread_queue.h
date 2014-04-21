#include "mypthreads.h"

typedef struct _threadnode {
    struct _threadnode *next;
    mypthread_t tid;
} threadnode;

typedef struct _queue {
    struct _threadnode *front;
    struct _threadnode *end;
} queue;

int myenqueue(mypthread_t);

mypthread_t mydequeue();

void myqueueinit();
