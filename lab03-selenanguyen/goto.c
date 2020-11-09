// Write a C program using the goto command and a label.
#include <stdio.h>

int main() {
	int num = 10;
	if (num % 2 == 0)
		goto even;
	else
		goto odd;

even:
	printf("%d is an even number!\n", num);
	return 0;
odd:
	printf("%d is an odd number!\n", num);
	return 0;
}
