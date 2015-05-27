/**********************************************
        CS415  Compilers  Homework 6
        author: U.Kremer  April 2015
**********************************************/

#include "memory.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static 
MemoryCell **Memory;

static
int hash(char *name) {
  int i;
  int hashValue = 1;
  
  for (i=0; i < strlen(name); i++) {
    hashValue = (hashValue * name[i]) % MEMORY_SIZE;
  }
  
  return hashValue;
}


void
InitMemory() {
  int i;

  Memory = (MemoryCell **) malloc (sizeof(MemoryCell *) * MEMORY_SIZE);
  for (i=0; i < MEMORY_SIZE; i++) {      
    Memory[i] = NULL;
  }
}


int  mem_get(char *id) 
{
  int currentIndex;
  currentIndex = hash(id);
  if (Memory[currentIndex] == NULL)
    {
      printf("\n*** WARNING: Read uninitialized variables \"%s\". Return value 0\n", id);
      mem_put(id, 0);
      
      return 0;
    }

  if (Memory[currentIndex] != NULL && strcmp(Memory[currentIndex]->name, id) ) 
    {
      printf("\n*** Memory conflict: Variables \"%s\" and \"%s\" map to same memory location %d\n", Memory[currentIndex]->name, id, currentIndex);
      exit (-1);
    }
    
  return( Memory[currentIndex]->value);   
  
}

void mem_put(char *id, int val) 
{
  int currentIndex;
  currentIndex = hash(id);
  if (Memory[currentIndex] != NULL && strcmp(Memory[currentIndex]->name, id) ) 
    {
      printf("\n*** Memory conflict: Variables %s and %s map to same memory location %d\n", Memory[currentIndex]->name, id, currentIndex);
      exit (-1);
    }
    
  Memory[currentIndex] = (MemoryCell *) malloc (sizeof(MemoryCell));
  Memory[currentIndex]->name = (char *) malloc (strlen(id)+1);
  strcpy(Memory[currentIndex]->name, id);
  Memory[currentIndex]->value = val; 
}

void 
MemoryDump() {
  int i;
  
  printf("\n ----------- MEMORY DUMP ----------\n");
  for (i=0; i < MEMORY_SIZE; i++) {
    if (Memory[i] != NULL) {
      printf("  %d:%s = %d\n", i, Memory[i]->name, Memory[i]->value);
    }
  }
  printf(" ----------------------------------\n");
  printf(" Maximal memory size: %d variables\n\n", MEMORY_SIZE);
  
}

