#include "dequeue.h"
#include <stdlib.h>

void initNode(Node node) {
    node->prev = node->next = node->data = NULL;
}

void initDequeue(Dequeue d) {
    d->left = d->right = NULL;
}

int dequeue_insert(Dequeue d, void *data, char end) {
    Node newNode;
    Node *endNodePtr;
    Node *newPtr;
    Node *endPtr;

    newNode = malloc(sizeof(struct Node));
    initNode(newNode);
    newNode->data = data;

    if (d->left == NULL) {
        d->left = d->right = newNode;
        return 0;
    }

    if (end == 'l')
    {
        endNodePtr = &d->left;
        newPtr = &newNode->next;
        endPtr = &(*endNodePtr)->prev;
    }
    else
    {
        endNodePtr = &d->right;
        newPtr = &newNode->prev;
        endPtr = &(*endNodePtr)->next;
    }

    *newPtr = *endNodePtr;
    *endPtr = newNode;
    *endNodePtr = newNode;

    return 0;
}

void* dequeue(Dequeue d, char end) {
    Node *endNodePtr;
    Node newEnd;
    void *data;

    if (d->left == NULL) {
        return NULL;
    }

    if (d->left == d->right) {
        data = d->left->data;
        free(d->left);
        d->left = d->right = NULL;
        return data;
    }

    if (end == 'l')
    {
        endNodePtr = &d->left;
        newEnd = (*endNodePtr)->next;
    }
    else
    {
        endNodePtr = &d->right;
        newEnd = (*endNodePtr)->prev;
    }

    data = (*endNodePtr)->data;
    free(*endNodePtr);
    *endNodePtr = newEnd;

    return data;
}

int linsert(Dequeue d, void *data) {
    return dequeue_insert(d, data, 'l');
}

int rinsert(Dequeue d, void *data) {
    return dequeue_insert(d, data, 'r');
}

void* ldequeue(Dequeue d) {
    return dequeue(d, 'l');
}

void* rdequeue(Dequeue d) {
    return dequeue(d, 'r');
}
