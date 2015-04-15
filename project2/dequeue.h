#ifndef DEQUEUE_H
#define DEQUEUE_H 1

struct Node {
    struct Node *prev;
    struct Node *next;
    void *data;
};

typedef struct Node* Node;

struct Dequeue{
    Node left;
    Node right;
};

typedef struct Dequeue* Dequeue;

void initDequeue(Dequeue);

int rinsert(Dequeue, void*);
int linsert(Dequeue, void*);

void* rdequeue(Dequeue);
void* ldequeue(Dequeue);

#endif
