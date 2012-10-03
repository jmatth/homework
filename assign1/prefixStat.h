#include <stdio.h>
//check the command line arguments
int readArgs(int argc, const char *argv[]);

//count the number of lines in the file
//takes the memory address of the file
//already loaded into memory
int lineCount(FILE *file);

//count the number of words in the file
//takes the memory address of the file
//already loaded into memory
int wordCount(FILE *file);

//print help message
void printHelp();
