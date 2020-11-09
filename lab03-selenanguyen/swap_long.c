// Now modify your program to swap two long's.

#include <stdio.h>
int main() {
        long x;
        long y;
        printf("Enter long  x: ");
        scanf("%ld", &x);
        printf("Enter long y: ");
        scanf("%ld", &y);
        printf("x = %ld, y = %ld\n", x, y);

        // Swap number:
        printf("Swapping longs...\n");
	long temp = x;
        x = y;
        y = temp;
	printf("x = %ld, y = %ld\n", x, y);
	return 0;
}
