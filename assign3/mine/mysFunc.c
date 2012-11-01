#include<stdio.h>
#include"mystery.h"

int main(int argc, char *argv[])
{
	int n;
	int result;

	if(argc != 2){
		printf("please check the usage of this program!\n");
		return;
	}
	
	n = atoi(argv[1]);
	result = mystery(n);

	printf("%d\n", result);
}
