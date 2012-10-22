#include <stdio.h>
#include <string.h>
#include "dataConvertor.h"

int main(int argc, char *argv[])
{

	//if readargs returns an error, print
	//the help message and exit
	if (readArgs(argc, argv)) {
		printHelp();
		return 1;
	}

	return 0;
}

int readArgs(int argc, char *argv[])
{
	//check for wrong number of arguments, call user an idiot
	if (argc != 4)
	{
		fprintf(stderr, "Wrong number of arguments.\n");
		return 1;
	}

	//error if we have flags that are more
	//than 2 characters long
	if (strlen(argv[1]) > 2 || strlen(argv[3]) > 2)
	{
		fprintf(stderr, "Invalid argument.\n");
		return 1;
	}

	//make sure the flags are correct
	if(argv[1][0] != '-' || argv[3][0] != '-')
	{
		fprintf(stderr, "Invalid flag.\n");
		return 1;
	}
	
	if((argv[1][1] != 'b' && argv[1][1] != 'd' && argv[1][1] != 'h' && argv[1][1] != 'o') || (argv[3][1] != 'b' && argv[3][1] != 'd' && argv[3][1] != 'h' && argv[3][1] != 'o'))
	{
		fprintf(stderr, "Invalid flag.\n");
		return 1;
	}
	return 0;
}


void printHelp() {
	printf("Usage: dataConvertor <input_type: -b|d|h|o> <data> <output_type: -b|d|h|o>\n");
	printf("Flags:\n");
	printf("\t-b:\t\tUse binary data for input or output.\n");
	printf("\t-d:\t\tUse decimal data for input or output.\n");
	printf("\t-h:\t\tUse hex data for input or output.\n");
	printf("\t-o:\t\tUse octal data for input or output.\n");
}
