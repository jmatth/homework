#include <stdio.h>
#include <string.h>
#include "prefixStat.h"

int main(int argc, const char *argv[])
{
	char action = readArgs(argc, argv);
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
