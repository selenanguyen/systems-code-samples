// Write a C program called array2.c that has an array of 400 integers in the function of main that is dynamically allocated.
#include <stdio.h>
#include <stdlib.h>

int main() {
	int *arr = (int*)malloc(sizeof(int)*400);
	arr[66] = 712;
	arr[70] = 536;
	arr[250] = 10;
	arr[350] = 300;
	return 0;
}
