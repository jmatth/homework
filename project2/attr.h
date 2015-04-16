/**********************************************
        CS415  Project 2
        Spring  2015
        Student Version
**********************************************/

#ifndef ATTR_H
#define ATTR_H

#include "dequeue.h"

typedef struct {
    int num;
    int cl;
    int sz;
    char *str;
    struct Dequeue dequeue;
} tokentype;

typedef enum type_expression {
    TYPE_INT=0,
    TYPE_BOOL,
    TYPE_ERROR
} Type_Expression;

typedef enum class_expression {
    CL_SCALAR=0,
    CL_ARR
} Class_Expression;

typedef struct {
    Type_Expression type;
    int targetRegister;
    int endLabel;
    int controlLabel;
} regInfo;

#endif
