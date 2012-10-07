#include <stdio.h>
//check the command line arguments
int readArgs(int argc, char *argv[]);

//count the number of lines in the file
//takes the memory address of the file
//already loaded into memory
int lineCount(FILE *file);

//count the number of words in the file
//takes the memory address of the file
//already loaded into memory
int wordCount(FILE *file);

//find the all words in file that start with
//prefix, ignoring case.
int findPrefix(FILE *file, char prefix[]);

// compare two ASCII characters ignoring case
int charCaseComp(char *one, char *two);

//print help message
void printHelp();
