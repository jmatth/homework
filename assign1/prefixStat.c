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
	if (argc > 4) {
		printf("Too many arguments\n");
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
				return 1;
			}
			//make sure the flags are correct
			if(argv[i][0] != '-'){
				printf("Invalid flag. No '-'.\n");
				return 1
			}
			switch(argv[i][1])
			{
				case 'h' :
					return 2;
					break;
				case 'w' :
					//some code here
					break;
				case 'l' :
					//some more code
					break;
				case 'p' :
					//last bit here
					break;
			}
		}
	}
}
