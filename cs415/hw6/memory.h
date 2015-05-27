/**********************************************
        CS415  Compilers  Homework 6
        author: U.Kremer  April 2015
**********************************************/

#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE 147

typedef struct {
  char *name;
  int  value;
} MemoryCell;

extern
void InitMemory();


extern
int  mem_get(char *id);

extern
void mem_put(char *id, int value);


#endif
