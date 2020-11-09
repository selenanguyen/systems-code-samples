// Write a C program using a switch statement
#include <stdio.h>
int main() {
	int num = 10;
	switch(num) {
		case 9 :
			printf("Nine.\n");
			break;
		case 10 :
			printf("Ten.\n");
			break;
		case 11 :
			printf("Eleven.\n");
			break;
		default :
			printf("Some other number.\n");
			break;
	}
	return 0;
}
