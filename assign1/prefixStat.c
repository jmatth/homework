#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prefixStat.h"

int main(int argc, char *argv[])
{

	//if readargs returns an error, print
	//the help message and exit
	if (readArgs(argc, argv)) {
		printHelp();
		return 1;
	}

	//load the file specified at the command line
	FILE *file;
	if (argv[1][1] != 'p') {
		file = fopen(argv[2], "r");
	}else{
		file = fopen(argv[3], "r");
	}

	//error out if the file didn't open
	if (file == NULL) {
		printf("File not found or not readable.\n");
		return 1;
	}
	switch(argv[1][1])
	{
		case 'h' :
			printHelp();
			break;
		case 'w' :
			wordCount(file);
			break;
		case 'l' :
			lineCount(file);
			break;
		case 'p' :
			findPrefix(file, argv[2]);
			break;
		default :
			printf("Well something got really fucked.\n");
			break;
	}

	//doing things the Right Way (TM)
	fclose(file);

	return 0;
}

int readArgs(int argc, char *argv[])
{
	//check for too many arguments, call user an idiot
	if ((argc != 3 && argv[1][1] != 'p') || (argc != 4 && argv[1][1] == 'p'))
	{
		printf("Wrong number of arguments.\n");
		return 1;
	}

	//error if we have flags that are more
	//than 2 characters long
	if (strlen(argv[1]) > 2)
	{
		printf("Long argument.\n");
		return 1;
	}

	//make sure the flags are correct
	if(argv[1][0] != '-')
	{
		printf("Invalid flag. No '-'.\n");
		return 1;
	}
	
	if(argv[1][1] != 'w' && argv[1][1] != 'l' && argv[1][1] != 'p' && argv[1][1] != 'h')
	{
		printf("Invalid flag.\n");
		return 1;
	}
	return 0;
}

int wordCount(FILE *file)
{
	//create a buffer
	char buff;

	//and a counter
	int count = 0;

	buff = fgetc(file);
	while (buff != EOF) {
		if (!isspace(buff)) {
			printf("%c",buff);
			++count;
			buff = fgetc(file);
			while (!isspace(buff) && buff != EOF) {
				printf("%c",buff);
				buff = fgetc(file);
			}
			printf(" ");
		}else {
			buff = fgetc(file);
		}
	}
	printf("\n%d\n",count);
	return 0;
}

int lineCount(FILE *file)
{
	printf("Counting lines.\n");
	
	//create a buffer
	char buff;

	//and a counter
	int count = 0;

	buff = fgetc(file);
	while (buff != EOF) {
		if (buff == '\n') {
			++count;
		}
		buff = fgetc(file);
	}
	printf("%d lines in file.\n",count);
	return 0;
}

int findPrefix(FILE *file, char string[])
{
	//allocate a buffer the same
	//size as the string
	int stringLen = strlen(string);
	char buff[stringLen];

	return 0;
}

void printHelp() {
	printf("Usage: prefixStat <options> <file>\n");
	printf("Options:\n");
	printf("\t-h:\tPrint this help message and exit.\n");
	printf("\t-l:\tPrint line count.\n");
	printf("\t-p <prefix>:\tFind words starting with the string <prefix>.\n");
	printf("\t-w:\tPrint word count.\n");
}
