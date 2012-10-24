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

	char *result = o2b(argv[2]);
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

char* o2b(char *input)
{
	char *binary = (char*) malloc(25);
	binary[26] = '\0';
	int i;
	int curr_ptr = 0;

	for (i = 0; i < strlen(input); i++) 
	{
		switch (toupper(input[i]))
		{
			case '.' :
				strcpy(binary+curr_ptr, "."); curr_ptr++; break;
			case '0' :
				strcpy(binary+curr_ptr, "000"); curr_ptr+=3; break;
			case '1' :
				strcpy(binary+curr_ptr, "001"); curr_ptr+=3; break;
			case '2' :
				strcpy(binary+curr_ptr, "010"); curr_ptr+=3; break;
			case '3' :
				strcpy(binary+curr_ptr, "011"); curr_ptr+=3; break;
			case '4' :
				strcpy(binary+curr_ptr, "100"); curr_ptr+=3; break;
			case '5' :
				strcpy(binary+curr_ptr, "101"); curr_ptr+=3; break;
			case '6' :
				strcpy(binary+curr_ptr, "110"); curr_ptr+=3; break;
			case '7' :
				strcpy(binary+curr_ptr, "111"); curr_ptr+=3; break;
		}
	}
	return binary;
}

char* h2b(char *input)
{
	char *binary = (char*) malloc(33);
	binary[34] = '\0';
	int i;
	int curr_ptr = 0;

	for (i = 0; i < strlen(input); i++) 
	{
		switch (toupper(input[i]))
		{
			case '.' :
				strcpy(binary+curr_ptr, "."); curr_ptr++; break;
			case '0' :
				strcpy(binary+curr_ptr, "0000"); curr_ptr+=4; break;
			case '1' :
				strcpy(binary+curr_ptr, "0001"); curr_ptr+=4; break;
			case '2' :
				strcpy(binary+curr_ptr, "0010"); curr_ptr+=4; break;
			case '3' :
				strcpy(binary+curr_ptr, "0011"); curr_ptr+=4; break;
			case '4' :
				strcpy(binary+curr_ptr, "0100"); curr_ptr+=4; break;
			case '5' :
				strcpy(binary+curr_ptr, "0101"); curr_ptr+=4; break;
			case '6' :
				strcpy(binary+curr_ptr, "0110"); curr_ptr+=4; break;
			case '7' :
				strcpy(binary+curr_ptr, "0111"); curr_ptr+=4; break;
			case '8' :
				strcpy(binary+curr_ptr, "1000"); curr_ptr+=4; break;
			case '9' :
				strcpy(binary+curr_ptr, "1001"); curr_ptr+=4; break;
			case 'A' :
				strcpy(binary+curr_ptr, "1010"); curr_ptr+=4; break;
			case 'B' :
				strcpy(binary+curr_ptr, "1011"); curr_ptr+=4; break;
			case 'C' :
				strcpy(binary+curr_ptr, "1100"); curr_ptr+=4; break;
			case 'D' :
				strcpy(binary+curr_ptr, "1101"); curr_ptr+=4; break;
			case 'E' :
				strcpy(binary+curr_ptr, "1110"); curr_ptr+=4; break;
			case 'F' :
				strcpy(binary+curr_ptr, "1111"); curr_ptr+=4; break;
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
