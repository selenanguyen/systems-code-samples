// Write a C program that swaps two integers(in the main body of code).
#include <stdio.h>
int main() {
	int x;
	int y;
	printf("Enter integer  x: ");
	scanf("%d", &x);
	printf("Enter integer y: ");
	scanf("%d", &y);
	printf("x = %d, y = %d\n", x, y);

	// Swap number:
	printf("Swapping integers...\n");
	int temp = x;
	x = y;
	y = temp;
	printf("x = %d, y = %d\n", x, y);
	
	return 0;
}
