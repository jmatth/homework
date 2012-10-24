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

	driver(argv[1][1], argv[2], argv[3][1]);

	return 0;
}

void driver(char inf, char *data, char outf)
{
	char *binary;

	if(inf == outf)
	{
		printf("%s\n", data);
		return;
	}

	switch(inf)
	{
		case 'b' :
			binary = data; break;
		case 'h' :
			binary = h2b(data); break;
		case 'o' :
			binary = o2b(data); break;
		case 'd' :
			binary = d2b(data); break;
	}

	switch(outf)
	{
		case 'b' :
			printf("%s\n", binary); break;
		case 'o' :
			printf("%s\n", b2oh(binary, 3)); break;
		case 'h' :
			printf("%s\n", b2oh(binary, 4)); break;
		case 'd' :
			printf("%g\n", b2d(binary)); break;
	}
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

double b2d(char *input)
{
	int i = 0;
	signed int found_point = -1;
	double ret;

	for (i = 0; i < strlen(input); i++) {
		if(input[i] == '.')
		{
			found_point = i;
			break;
		}
	}

	if(found_point >= 0)
	{
		char before_point[found_point+1];
		before_point[found_point] = '\0';
		strncpy(before_point, input, found_point);

		char after_point[strlen(input)-found_point+1];
		strcpy(after_point, input+found_point+1);

		int before_point_len = strlen(before_point);
		int after_point_len = strlen(after_point);


		for (i = 0; i < before_point_len; i++) 
		{
			if(before_point[i] == '1')
			{
				ret+=pow(2, (before_point_len - (i+1)));
			}else
			{
				continue;
			}
		}


		for (i = 0; i < after_point_len; i++) 
		{
			if(after_point[i] == '1')
			{
				ret += pow(2, -(i+1));
			}
		}

		return ret;

	}
	else
	{
		int input_length = strlen(input);

		for (i = 0; i < input_length; i++) 
		{
			if(input[i] == '1')
			{
				ret += pow(2, input_length-(i+1));
			}else
			{
				continue;
			}
		}


		return ret;
		
	}
}

char* b2oh(char *input, int hoo)
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
		char before_point[found_point+1];
		before_point[found_point] = '\0';
		strncpy(before_point, input, found_point);
		char *before_point_ptr = before_point;

		char *hex_ret = (char*) malloc(32);
		hex_ret[0] = '\0';
		hex_ret[33] = '\0';

		char after_point[strlen(input)-found_point+1];
		strcpy(after_point, input+found_point+1);
		char *after_point_ptr = after_point;

		int before_point_len = strlen(before_point);
		int before_point_mod = before_point_len % hoo;

		char before_point_pad[before_point_mod + before_point_len + 1];

		int after_point_len = strlen(after_point);
		int after_point_mod = after_point_len % hoo;

		char after_point_pad[after_point_mod + after_point_len];

		if(before_point_mod != 0)
		{
			before_point_mod = hoo - before_point_mod;
			for (i = 0; i < before_point_mod; i++) {
				before_point_pad[i] = '0';
			}
			strcpy(before_point_pad+before_point_mod, before_point);
			before_point_ptr = before_point_pad;
		}

		char before_point_hex[16];
		before_point_hex[0] = '\0';
		char curr_nibble[hoo];
		for (i = 0; i < strlen(before_point_ptr); i+=hoo) 
		{
			strncpy(curr_nibble, before_point_ptr+i, hoo);
			curr_nibble[hoo] = '\0';

			if(hoo == 4)
			{

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
			} else if(hoo == 3)
			{
				if(!strcmp(curr_nibble, "000"))
				{
					strcat(before_point_hex, "0");
				} else if (!strcmp(curr_nibble, "001"))
				{
					strcat(before_point_hex, "1");
				} else if (!strcmp(curr_nibble, "010"))
				{
					strcat(before_point_hex, "2");
				} else if (!strcmp(curr_nibble, "011"))
				{
					strcat(before_point_hex, "3");
				} else if (!strcmp(curr_nibble, "100"))
				{
					strcat(before_point_hex, "4");
				} else if (!strcmp(curr_nibble, "101"))
				{
					strcat(before_point_hex, "5");
				} else if (!strcmp(curr_nibble, "110"))
				{
					strcat(before_point_hex, "6");
				} else if (!strcmp(curr_nibble, "111"))
				{
					strcat(before_point_hex, "7");
				}
			}

		}

		if(after_point_mod != 0)
		{
			after_point_mod = hoo - after_point_mod;
			for (i = after_point_mod + after_point_len - 1; i > after_point_mod; i--) 
			{
				after_point_pad[i] = '0';
				strncpy(after_point_pad, after_point, after_point_len);
				after_point_ptr = after_point_pad;
			}
		}

		char after_point_hex[16];
		after_point_hex[0] = '\0';
		for (i = 0; i < strlen(after_point_ptr); i+=hoo) 
		{
			strncpy(curr_nibble, after_point_ptr+i, hoo);
			curr_nibble[hoo] = '\0';

			if(hoo == 4)
			{

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
			}else if(hoo == 3)
			{
				if(!strcmp(curr_nibble, "000"))
				{
					strcat(after_point_hex, "0");
				} else if (!strcmp(curr_nibble, "001"))
				{
					strcat(after_point_hex, "1");
				} else if (!strcmp(curr_nibble, "010"))
				{
					strcat(after_point_hex, "2");
				} else if (!strcmp(curr_nibble, "011"))
				{
					strcat(after_point_hex, "3");
				} else if (!strcmp(curr_nibble, "100"))
				{
					strcat(after_point_hex, "4");
				} else if (!strcmp(curr_nibble, "101"))
				{
					strcat(after_point_hex, "5");
				} else if (!strcmp(curr_nibble, "110"))
				{
					strcat(after_point_hex, "6");
				} else if (!strcmp(curr_nibble, "111"))
				{
					strcat(after_point_hex, "7");
				}
			}
		}

		strcat(hex_ret, before_point_hex);
		strcat(hex_ret, ".");
		strcat(hex_ret, after_point_hex);

		return hex_ret;

	}
	else
	{
		char *hex = (char*) malloc(17);
		hex[0] = '\0';
		hex[16] = '\0';
		int input_length = strlen(input);
		int input_mod = input_length % hoo;
		char *input_ptr = input;
		char input_pad[input_length + input_mod + 1];

		if(input_mod != 0)
		{
			input_mod = hoo - input_mod;
			input_pad[input_length + input_mod] = '\0';

			for (i = 0; i < input_mod; i++) {
				input_pad[i] = '0';
			}

			input_pad[i] = '\0';
			strcat(input_pad, input);
			input_ptr = input_pad;

		}

		char curr_nibble[5];
		curr_nibble[hoo] = '\0';
		for (i = 0; i < strlen(input_ptr); i+=hoo) 
		{

			strncpy(curr_nibble, input_ptr+i, hoo);

			curr_nibble[hoo] = '\0';


			if(hoo == 4)
			{

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
			}else if(hoo == 3)
			{
				if(!strcmp(curr_nibble, "000"))
				{
					strcat(hex, "0");
				} else if (!strcmp(curr_nibble, "001"))
				{
					strcat(hex, "1");
				} else if (!strcmp(curr_nibble, "010"))
				{
					strcat(hex, "2");
				} else if (!strcmp(curr_nibble, "011"))
				{
					strcat(hex, "3");
				} else if (!strcmp(curr_nibble, "100"))
				{
					strcat(hex, "4");
				} else if (!strcmp(curr_nibble, "101"))
				{
					strcat(hex, "5");
				} else if (!strcmp(curr_nibble, "110"))
				{
					strcat(hex, "6");
				} else if (!strcmp(curr_nibble, "111"))
				{
					strcat(hex, "7");
				}
			}
		}



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

double my_atoi(char *input)
{
	short int point = 0;
	int input_len = strlen(input);
	int i;
	double ret;

	for (i = 0; i < input_len; i++) 
	{
		if(input[i] == '.')
		{
			point = 1;
			continue;
		}

		if(!point)
		{
			if(input[i] == '1')
			{
				ret += 1 * pow(10, input_len-(i+1));
			}else if(input[i] == '2')
			{
				ret += 2 * pow(10, input_len-(i+1));
			}else if(input[i] == '3')
			{
				ret += 3 * pow(10, input_len-(i+1));
			}else if(input[i] == '4')
			{
				ret += 4 * pow(10, input_len-(i+1));
			}else if(input[i] == '5')
			{
				ret += 5 * pow(10, input_len-(i+1));
			}else if(input[i] == '6')
			{
				ret += 6 * pow(10, input_len-(i+1));
			}else if(input[i] == '7')
			{
				ret += 7 * pow(10, input_len-(i+1));
			}else if(input[i] == '8')
			{
				ret += 8 * pow(10, input_len-(i+1));
			}else if(input[i] == '9')
			{
				ret += 9 * pow(10, input_len-(i+1));
			}else
			{
				continue;
			}
		}
		else
		{
			if(input[i] == '1')
			{
				ret += 1 * pow(10, -i);
			}else if(input[i] == '2')
			{
				ret += 2 * pow(10, -i);
			}else if(input[i] == '3')
			{
				ret += 3 * pow(10, -i);
			}else if(input[i] == '4')
			{
				ret += 4 * pow(10, -i);
			}else if(input[i] == '5')
			{
				ret += 5 * pow(10, -i);
			}else if(input[i] == '6')
			{
				ret += 6 * pow(10, -i);
			}else if(input[i] == '7')
			{
				ret += 7 * pow(10, -i);
			}else if(input[i] == '8')
			{
				ret += 8 * pow(10, -i);
			}else if(input[i] == '9')
			{
				ret += 9 * pow(10, -i);
			}else
			{
				continue;
			}
		}
	}
	return ret;
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
		double integer = my_atoi(before_dec);

		char *after_dec = strdup(input+point);
		double decimal = my_atoi(after_dec);

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

		double integer = my_atoi(input);
		ret_str = get_binary_integer(integer);

	}

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
