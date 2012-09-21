#include <stdio.h>
#include <string.h>
#include "prefixStat.h"

int main(int argc, const char *argv[])
{
	readArgs(argc, argv);
	return 0;
}

int readArgs(int argc, const char *argv[])
{
	//check for too many arguments, call user an idiot
	if (argc != 3) {
		printf("Wrong number of arguments.\n");
		return 1;
	}

	//if there are more arguments than just
	//commmand and file name
	if (argc > 2) {
		int i;
		for (i = 1; i < argc-1; i++) {
			//error if we have flags that are more
			//than 2 characters long
			if (strlen(argv[i]) > 2) {
				printf("Long argument.\n");
				return 0;
			}
			//make sure the flags are correct
			if(argv[i][0] != '-'){
				printf("Invalid flag. No '-'.\n");
				return 0;
			}
			switch(argv[i][1])
			{
				case 'w' :
					return 2;
					break;
				case 'l' :
					return 3;
					break;
				case 'p' :
					return 1;
					break;
				default :
					return 0;
					break;
			}
		}
	}
}
