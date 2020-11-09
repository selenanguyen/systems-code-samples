#include <stdio.h>
void swap(int* x, int* y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

int main() {
	int x;
	int y;
	printf("Enter integer  x: ");
	scanf("%d", &x);
	printf("Enter integer y: ");
	scanf("%d", &y);
	printf("x = %d, y = %d\n", x, y);
	
	printf("Swapping x and y...");
	swap(&x, &y);
	printf("x = %d, y = %d\n", x, y);
	return 0;
}
