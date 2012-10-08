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

	//load the file
	FILE *file;
	file = fopen("test.dat", "r");

	//error out if the file didn't open
	if (file == NULL) {
		fprintf(stderr, "File not found or not readable.\n");
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
			fprintf(stderr, "An unknown error occurred.\n");
			break;
	}

	//doing things the Right Way (TM)
	fclose(file);

	return 0;
}

int readArgs(int argc, char *argv[])
{
	//check for wrong number of arguments, call user an idiot
	if ((argc < 2) || (argc != 2 && argv[1][1] != 'p') || (argc != 3 && argv[1][1] == 'p'))
	{
		fprintf(stderr, "Wrong number of arguments.\n");
		return 1;
	}

	//error if we have flags that are more
	//than 2 characters long
	if (strlen(argv[1]) > 2)
	{
		fprintf(stderr, "Invalid argument.\n");
		return 1;
	}

	//make sure the flags are correct
	if(argv[1][0] != '-')
	{
		fprintf(stderr, "Invalid flag.\n");
		return 1;
	}
	
	if(argv[1][1] != 'w' && argv[1][1] != 'l' && argv[1][1] != 'p' && argv[1][1] != 'h')
	{
		fprintf(stderr, "Invalid flag.\n");
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
	//create a buffer
	char buff;

	//and a counter
	int count = 0;

	//check to fix eol errors.
	//some systems automatically add
	//an eol at the end of a file, others
	//don't. to fix this we keep track of
	//wheather the file ended in an eol
	short int endLine=0;

	buff = fgetc(file);
	while (buff != EOF) {
		if (buff == '\n') {
			endLine = 1;
			++count;
		}else {
			endLine = 0;
		}
		printf("%c",buff);
		buff = fgetc(file);
	}
	if(!endLine) {
		++count;
		printf("\n");
	}

	printf("%d\n",count);
	return 0;
}

int findPrefix(FILE *file, char prefix[])
{
	//create a buffer
	char buff;

	//and a counter
	int count = 0;

	//current index in the prefix
	int index = 0;

	//store the lenght of
	//the prefix prefix
	int prelen = strlen(prefix);

	//store the prefix as it
	//actually appears in the word
	char verbatimBuff[prelen+1];
	verbatimBuff[prelen] = '\0';

	//see if we need to print a
	//space or not
	short int first = 1;

	buff = fgetc(file);

	while (buff != EOF) {
		if (isspace(buff)) {
			buff = fgetc(file);
			continue;
		}

		//if (buff == prefix[index]) {
		if (charCaseComp(&buff, &prefix[index])) {
			verbatimBuff[index] = buff;
			buff = fgetc(file);
			++index;
		}else {
			index = 0;
			while (!isspace(buff) && buff != EOF) {
				buff = fgetc(file);
			}
			continue;
		}

		//we've gone over, so the prefix
		//matches. print the word and reset
		//the index
		if (index >= prelen) {
			++count;
			if (first) {
				printf("%s", verbatimBuff);
				first = 0;
			}else{
				printf(" %s", verbatimBuff);
			}

			while(!isspace(buff) && buff != EOF){
				printf("%c", buff);
				buff = fgetc(file);
			}
			index = 0;
		}
	}

	printf("\n%d\n",count);

	return 0;
}

void printHelp() {
	printf("Usage: prefixStat <options> <file>\n");
	printf("Options:\n");
	printf("\t-h:\t\tPrint this help message and exit.\n");
	printf("\t-l:\t\tPrint line count.\n");
	printf("\t-p <prefix>:\tFind words starting with the string <prefix>.\n");
	printf("\t-w:\t\tPrint word count.\n");
}

int charCaseComp (char *one, char *two) {

	//just check for equivalence first
	if (*one == *two) {
		return 1;
	}

	//they're not equal. if they're letters
	//then check if they're just different
	//cases
	if (*one >= 'A' && *one <= 'Z') {
		if (*one + 32 == *two ) {
			return 1;
		}
	} else if (*one >= 'a' && *one <= 'z') {
		if (*one - 32 == *two) {
			return 1;
		}
	}

	//nothing worked, they don't match
	return 0;
}
