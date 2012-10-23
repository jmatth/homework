#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "dataConvertor.h"

int main(int argc, char *argv[])
{

	//if readargs returns an error, print
	//the help message and exit
	if (readArgs(argc, argv)) {
		printHelp();
		return 1;
	}

	d2b(argv[2]);

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

char* d2b(char *input)
{
	int i = 0;
	int point = -1;
	char *ret_str;
	for (i = 0; i < strlen(input); i++) {
		if (input[i] == '.') {
			point = i;
			break;
		}
	}
	if (point >= 0)
	{
		char *before_dec;
		before_dec = strndup(input, point);
		int integer = atoi(before_dec);
		char *after_dec;
		char *end;
		after_dec = strndup(input+point, strlen(input));
		double decimal = strtof(after_dec, &end);
		free(before_dec);
		free(after_dec);
		char *integer_string = get_integer(integer);
		char *decimal_string = get_decimal(decimal);
		ret_str = (char*) malloc(strlen(integer_string) + strlen(decimal_string) + 2);
		strcat(ret_str, integer_string);
		strcat(ret_str, ".");
		strcat(ret_str, decimal_string);
		free(integer_string);
		free(decimal_string);
		
		
	}
	printf("%s\n", ret_str);
	return ret_str;
}

char* get_integer(int integer)
{
	char *binstr = (char*) malloc(9);
	int i = 0;
	for (i = 0; i < 9; i++) {
		binstr[i] = '0';
	}
	binstr[8] = '\0';

	for (i = 0; i < 9; i++) {
		if (integer - pow((double)2, (double)7-i) >= 0) {
			binstr[i] = '1';
			integer = integer - pow((double)2, (double)7-i);
		}
		if (integer == 0) {
			break;
		}
	}
	return binstr;

}

char* get_decimal(double decimal)
{
	char *decstr = (char*) malloc(9);
	int i;
	for (i = 0; i < 9; i++) {
		decstr[i] = '0';
	}
	decstr[8] = '\0';
	for (i = 0; i < 9; i++) {
		if (decimal - pow(2, -(i+1)) >= 0) {
			decstr[i] = '1';
			decimal = decimal - pow(2, -(i+1)); 
		}
		if (decimal == 0) {
			break;
		}
	}

	return decstr;
}

void printHelp() {
	printf("Usage: dataConvertor <input_type: -b|d|h|o> <data> <output_type: -b|d|h|o>\n");
	printf("Flags:\n");
	printf("\t-b:\t\tUse binary data for input or output.\n");
	printf("\t-d:\t\tUse decimal data for input or output.\n");
	printf("\t-h:\t\tUse hex data for input or output.\n");
	printf("\t-o:\t\tUse octal data for input or output.\n");
}
