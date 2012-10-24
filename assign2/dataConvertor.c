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

	char *result = argv[2];
	printf("Hex to binary is %s\n", result);
	char *more = b2h(result);
	printf("Second result is %s\n", more);

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

char* b2h(char *input)
{
	int i = 0;
	signed int found_point = -1;

	for (i = 0; i < strlen(input); i++) {
		if(input[i] == '.')
		{
			found_point = i;
			break;
		}
	}

	if(found_point >= 0)
	{
		printf("found point at %d\n", found_point);
		char before_point[found_point+1];
		before_point[found_point] = '\0';
		strncpy(before_point, input, found_point);
		printf("before point copies to %s\n", before_point);
		char *before_point_ptr = before_point;
		printf("ptr goes to %s\n", before_point_ptr);

		char *hex_ret = (char*) malloc(32);
		hex_ret[0] = '\0';
		hex_ret[33] = '\0';

		char after_point[strlen(input)-found_point+1];
		strcpy(after_point, input+found_point+1);
		char *after_point_ptr = after_point;

		int before_point_len = strlen(before_point);
		int before_point_mod = before_point_len % 4;

		char before_point_pad[before_point_mod + before_point_len + 1];

		int after_point_len = strlen(after_point);
		int after_point_mod = after_point_len % 4;

		char after_point_pad[after_point_mod + after_point_len];

		if(before_point_mod != 0)
		{
			before_point_mod = 4 - before_point_mod;
			printf("padding before point with %d\n", before_point_mod);
			for (i = 0; i < before_point_mod; i++) {
				before_point_pad[i] = '0';
			}
			strcpy(before_point_pad+before_point_mod, before_point);
			before_point_ptr = before_point_pad;
		}
		printf("before_point_ptr is %s\n", before_point_ptr);
		int tmp_size = strlen(before_point_ptr);
		printf("strlen is %d\n", tmp_size);

		char before_point_hex[16];
		before_point_hex[0] = '\0';
		char curr_nibble[4];
		for (i = 0; i < strlen(before_point_ptr); i+=4) 
		{
			strncpy(curr_nibble, before_point_ptr+i, 4);
			curr_nibble[4] = '\0';
			printf("curr nibble is now %s\n", curr_nibble);

			if(!strcmp(curr_nibble, "0000"))
			{
				strcat(before_point_hex, "0");
			} else if (!strcmp(curr_nibble, "0001"))
			{
				strcat(before_point_hex, "1");
			} else if (!strcmp(curr_nibble, "0010"))
			{
				strcat(before_point_hex, "2");
			} else if (!strcmp(curr_nibble, "0011"))
			{
				strcat(before_point_hex, "3");
			} else if (!strcmp(curr_nibble, "0100"))
			{
				strcat(before_point_hex, "4");
			} else if (!strcmp(curr_nibble, "0101"))
			{
				strcat(before_point_hex, "5");
			} else if (!strcmp(curr_nibble, "0110"))
			{
				strcat(before_point_hex, "6");
			} else if (!strcmp(curr_nibble, "0111"))
			{
				strcat(before_point_hex, "7");
			} else if (!strcmp(curr_nibble, "1000"))
			{
				strcat(before_point_hex, "8");
			} else if (!strcmp(curr_nibble, "1001"))
			{
				strcat(before_point_hex, "9");
			} else if (!strcmp(curr_nibble, "1010"))
			{
				strcat(before_point_hex, "A");
			} else if (!strcmp(curr_nibble, "1011"))
			{
				strcat(before_point_hex, "B");
			} else if (!strcmp(curr_nibble, "1100"))
			{
				strcat(before_point_hex, "C");
			} else if (!strcmp(curr_nibble, "1101"))
			{
				strcat(before_point_hex, "D");
			} else if (!strcmp(curr_nibble, "1110"))
			{
				strcat(before_point_hex, "E");
			} else if (!strcmp(curr_nibble, "1111"))
			{
				strcat(before_point_hex, "F");
			}


		}
		printf("i exited at %d\n", i);

		if(after_point_mod != 0)
		{
			printf("needs padding\n");
			after_point_mod = 4 - after_point_mod;
			for (i = after_point_mod + after_point_len - 1; i > after_point_mod; i--) 
			{
				after_point_pad[i] = '0';
				strncpy(after_point_pad, after_point, after_point_len);
				after_point_ptr = after_point_pad;
			}
		}

		char after_point_hex[16];
		after_point_hex[0] = '\0';
		for (i = 0; i < strlen(after_point_ptr); i+=4) 
		{
			strncpy(curr_nibble, after_point_ptr+i, 4);
			curr_nibble[4] = '\0';

			if(!strcmp(curr_nibble, "0000"))
			{
				strcat(after_point_hex, "0");
			} else if (!strcmp(curr_nibble, "0001"))
			{
				strcat(after_point_hex, "1");
			} else if (!strcmp(curr_nibble, "0010"))
			{
				strcat(after_point_hex, "2");
			} else if (!strcmp(curr_nibble, "0011"))
			{
				strcat(after_point_hex, "3");
			} else if (!strcmp(curr_nibble, "0100"))
			{
				strcat(after_point_hex, "4");
			} else if (!strcmp(curr_nibble, "0101"))
			{
				strcat(after_point_hex, "5");
			} else if (!strcmp(curr_nibble, "0110"))
			{
				strcat(after_point_hex, "6");
			} else if (!strcmp(curr_nibble, "0111"))
			{
				strcat(after_point_hex, "7");
			} else if (!strcmp(curr_nibble, "1000"))
			{
				strcat(after_point_hex, "8");
			} else if (!strcmp(curr_nibble, "1001"))
			{
				strcat(after_point_hex, "9");
			} else if (!strcmp(curr_nibble, "1010"))
			{
				strcat(after_point_hex, "A");
			} else if (!strcmp(curr_nibble, "1011"))
			{
				strcat(after_point_hex, "B");
			} else if (!strcmp(curr_nibble, "1100"))
			{
				strcat(after_point_hex, "C");
			} else if (!strcmp(curr_nibble, "1101"))
			{
				strcat(after_point_hex, "D");
			} else if (!strcmp(curr_nibble, "1110"))
			{
				strcat(after_point_hex, "E");
			} else if (!strcmp(curr_nibble, "1111"))
			{
				strcat(after_point_hex, "F");
			}
		}

		printf("before point comes to %s\n", before_point_hex);
		printf("after point comes to %s\n", after_point_hex);
		strcat(hex_ret, before_point_hex);
		strcat(hex_ret, ".");
		strcat(hex_ret, after_point_hex);

		printf("final value is %s\n", hex_ret);
		return hex_ret;

	}
	else
	{
		char *hex = (char*) malloc(17);
		hex[0] = '\0';
		hex[16] = '\0';
		int input_length = strlen(input);
		printf("input length is %d\n", input_length);
		int input_mod = input_length % 4;
		char *input_ptr = input;
		char input_pad[input_length + input_mod + 1];

		if(input_mod != 0)
		{
			printf("padding input.\n");
			input_mod = 4 - input_mod;
			printf("input mod is %d\n", input_mod);
			input_pad[input_length + input_mod] = '\0';

			for (i = 0; i < input_mod; i++) {
				input_pad[i] = '0';
			}

			input_pad[i] = '\0';
			strcat(input_pad, input);
			input_ptr = input_pad;

			printf("input_pad is %s\n", input_pad);
			printf("input_ptr is %s\n", input_ptr);
		}

		char curr_nibble[5];
		curr_nibble[4] = '\0';
		for (i = 0; i < strlen(input_ptr); i+=4) 
		{
			printf("copying from %s\n", input_ptr);

			strncpy(curr_nibble, input_ptr+i, 4);

			curr_nibble[4] = '\0';

			printf("curr nibble is %s\n", curr_nibble);

			if(!strcmp(curr_nibble, "0000"))
			{
				strcat(hex, "0");
			} else if (!strcmp(curr_nibble, "0001"))
			{
				strcat(hex, "1");
			} else if (!strcmp(curr_nibble, "0010"))
			{
				strcat(hex, "2");
			} else if (!strcmp(curr_nibble, "0011"))
			{
				strcat(hex, "3");
			} else if (!strcmp(curr_nibble, "0100"))
			{
				strcat(hex, "4");
			} else if (!strcmp(curr_nibble, "0101"))
			{
				strcat(hex, "5");
			} else if (!strcmp(curr_nibble, "0110"))
			{
				strcat(hex, "6");
			} else if (!strcmp(curr_nibble, "0111"))
			{
				strcat(hex, "7");
			} else if (!strcmp(curr_nibble, "1000"))
			{
				strcat(hex, "8");
			} else if (!strcmp(curr_nibble, "1001"))
			{
				strcat(hex, "9");
			} else if (!strcmp(curr_nibble, "1010"))
			{
				strcat(hex, "A");
			} else if (!strcmp(curr_nibble, "1011"))
			{
				strcat(hex, "B");
			} else if (!strcmp(curr_nibble, "1100"))
			{
				strcat(hex, "C");
			} else if (!strcmp(curr_nibble, "1101"))
			{
				strcat(hex, "D");
			} else if (!strcmp(curr_nibble, "1110"))
			{
				strcat(hex, "E");
			} else if (!strcmp(curr_nibble, "1111"))
			{
				strcat(hex, "F");
			}


		}


		printf("Hex result is %s\n", hex);

		return hex;
		
	}
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
	for (i = 0; i < strlen(input); i++) 
	{
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
