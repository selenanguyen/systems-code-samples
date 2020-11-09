// Write a C program that calls an add function(long add(long a, long b).
#include <stdio.h>
long add(long a, long b) {
	return a + b;
}

int main() {
	long a = 400;
	long b = 500;
	long sum = add(a, b);
	printf("%ld\n", sum);
	return 0;
}
