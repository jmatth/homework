#include <stdio.h>
#include <string.h>
#include "prefixStat.h"

int main(int argc, const char *argv[])
{
	char action = readArgs(argc, argv);

	if (action == 'h') {
		printHelp();
		return 0;
	}

	//load the file specified at the command line
	FILE *file = fopen(argv[2], "r");

	//error out if the file didn't open
	if (file == NULL) {
		printf("File not found or not readable.\n");
		return 1;
	}

	switch(action)
	{
		case 'w' :
			//code here
			break;
		default :
			printf("Well something got really fucked.\n");
			break;
	}

	return 0;
}

char readArgs(int argc, const char *argv[])
{
	//check for too many arguments, call user an idiot
	if (argc != 3)
	{
		printf("Wrong number of arguments.\n");
		return 'h';
	}

	//error if we have flags that are more
	//than 2 characters long
	if (strlen(argv[1]) > 2)
	{
		printf("Long argument.\n");
		return 'h';
	}

	//make sure the flags are correct
	if(argv[1][0] != '-')
	{
		printf("Invalid flag. No '-'.\n");
		return 'h';
	}

	switch(argv[1][1])
	{
		case 'w' :
			return 'w';
			break;
		case 'l' :
			return 'l';
			break;
		case 'p' :
			return 'p';
			break;
		default :
			printf("Invalid flag.\n");
			return 'h';
			break;
	}
}

void printHelp() {
	printf("Usage: prefixStat <options> <file>\n");
	printf("Options:\n");
	printf("\t-h:\tPrint this help message and exit.\n");
	printf("\t-l:\tPrint line count.\n");
	printf("\t-p:\tFind words starting with a prefix.\n");
	printf("\t-w:\tPrint word count.\n");
}
