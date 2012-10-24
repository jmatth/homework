#include <stdio.h>
//check the command line arguments
int readArgs(int argc, char *argv[]);

//print help message
void printHelp();

char* d2b(char *input);

char* h2b(char *input);

char* o2b(char *input);

char* b2oh(char *input, int hoo);

double b2d(char *input);

void driver(char inf, char *data, char outf);

char* get_binary_integer(int integer);

char* get_binary_decimal(double integer);
