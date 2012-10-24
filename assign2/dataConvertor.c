#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dataConvertor.h"

int main(int argc, char *argv[])
{

	//if readargs returns an error, print
	//the help message and exit
	if (readArgs(argc, argv)) {
		printHelp();
		return 1;
	}

	char *result = h2b(argv[2]);
	printf("%s\n", result);
	free(result);

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

char* h2b(char *input)
{
	char *binary = (char*) malloc(33);
	binary[34] = '\0';
	int i;

	for (i = 0; i < strlen(input); i++) 
	{
		switch (toupper(input[i]))
		{
			case '0' :
				strcpy(binary+(4*i), "0000"); break;
			case '1' :
				strcpy(binary+(4*i), "0001"); break;
			case '2' :
				strcpy(binary+(4*i), "0010"); break;
			case '3' :
				strcpy(binary+(4*i), "0011"); break;
			case '4' :
				strcpy(binary+(4*i), "0100"); break;
			case '5' :
				strcpy(binary+(4*i), "0101"); break;
			case '6' :
				strcpy(binary+(4*i), "0110"); break;
			case '7' :
				strcpy(binary+(4*i), "0111"); break;
			case '8' :
				strcpy(binary+(4*i), "1000"); break;
			case '9' :
				strcpy(binary+(4*i), "1001"); break;
			case 'A' :
				strcpy(binary+(4*i), "1010"); break;
			case 'B' :
				strcpy(binary+(4*i), "1011"); break;
			case 'C' :
				strcpy(binary+(4*i), "1100"); break;
			case 'D' :
				strcpy(binary+(4*i), "1101"); break;
			case 'E' :
				strcpy(binary+(4*i), "1110"); break;
			case 'F' :
				strcpy(binary+(4*i), "1111"); break;
		}
	}

	return binary;

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
		char *integer_string = get_binary_integer(integer);
		char *decimal_string = get_binary_decimal(decimal);
		ret_str = (char*) malloc(strlen(integer_string) + strlen(decimal_string) + 2);
		strcat(ret_str, integer_string);
		strcat(ret_str, ".");
		strcat(ret_str, decimal_string);
		free(integer_string);
		free(decimal_string);
	} else {
		int integer = atoi(input);
		ret_str = get_binary_integer(integer);
	}

	free(ret_str);
	return ret_str;
}

char* get_binary_integer(int integer)
{
	char binstr[27];
	int i = 0;
	int first_one = 0;
	
	for (i = 0; i < 27; i++) {
		binstr[i] = '0';
	}

	for (i = 0; i < 27; i++) {
		if (integer - pow((double)2, (double)26-i) >= 0) {
			binstr[i] = '1';
			integer = integer - pow((double)2, (double)26-i);
			if (! first_one) {
				first_one = i;
			}
		}
		if (integer == 0) {
			break;
		}
	}
	char *ret_binstr = (char*) malloc(27-first_one+2);
	strcpy(ret_binstr, binstr+first_one);
	return ret_binstr;
}

char* get_binary_decimal(double decimal)
{
	char decstr[28];
	int i;
	for (i = 0; i < 28; i++) {
		decstr[i] = '0';
	}
	for (i = 0; i < 28; i++) {
		if (decimal - pow(2, -(i+1)) >= 0) {
			decstr[i] = '1';
			decimal = decimal - pow(2, -(i+1)); 
		}
		if (decimal == 0) {
			break;
		}
	}
	char *ret_decstr = (char*) malloc(i+2);
	ret_decstr = strncat(ret_decstr, decstr, i+1);
	ret_decstr[i+2] = '\0';

	return ret_decstr;
}

void printHelp() {
	printf("Usage: dataConvertor <input_type: -b|d|h|o> <data> <output_type: -b|d|h|o>\n");
	printf("Flags:\n");
	printf("\t-b:\t\tUse binary data for input or output.\n");
	printf("\t-d:\t\tUse decimal data for input or output.\n");
	printf("\t-h:\t\tUse hex data for input or output.\n");
	printf("\t-o:\t\tUse octal data for input or output.\n");
}
