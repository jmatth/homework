/**********************************************
        CS415  Project 2
        Spring  2015
        Student Version
**********************************************/

#ifndef ATTR_H
#define ATTR_H

#include "dequeue.h"

typedef union {
    int num;
    char *str;
    struct Dequeue dequeue;
} tokentype;

typedef enum type_expression {
    TYPE_INT=0,
    TYPE_BOOL,
    TYPE_ERROR
} Type_Expression;

typedef struct {
    Type_Expression type;
    int targetRegister;
} regInfo;

#endif
