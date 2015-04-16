/**********************************************
        CS415  Project 2
        Spring  2015
        Student Version
**********************************************/


#ifndef SYMTAB_H
#define SYMTAB_H

#include <string.h>
#include "attr.h"

/* The symbol table implementation uses a single hash     */
/* function. Starting from the hashed position, entries   */
/* are searched in increasing index order until a         */
/* matching entry is found, or an empty entry is reached. */


typedef struct { /* need to augment this */
  char *name;
  int offset;
  int indices;
  Type_Expression type;
  Class_Expression cl;
} SymTabEntry;

extern
void InitSymbolTable();

extern
SymTabEntry * lookup(char *name);

extern
void insert(char *name, Type_Expression type, int offset, Class_Expression cl, int indices);

extern
void PrintSymbolTable();


#endif
