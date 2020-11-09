// Simple Caeser Shift decrypt
// 
// Load the data from data2.txt. Increment all of the values by 1 then output
// the results to stdout on the terminal.
// 
// You will compile this source using the following:
// clang -std=c11 -mavx2 ex2.c -o ex2prog
//
// Run as normal with: 
// ./ex2prog
//
#include <stdio.h>
#include <immintrin.h>



// print (Note, you may need to change this function or write another for your data)
void print__m256i(__m256i data){
	char*c = (char*)&data;
	printf("%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n", c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7],
		c[8],c[9],c[10],c[11],c[12],c[13],c[14],c[15],
		c[16],c[17],c[18],c[19],c[20],c[21],c[22],c[23],
		c[24],c[25],c[26],c[27],c[28],c[29],c[30],c[31]
		);
}

int main(){
	// Open the file
	FILE *fp;
        int numArray[32];
        fp = fopen("./data2.txt", "r");
        int i = 31;
        if(fp == NULL) {
                printf("Can't open file\n");
                exit(EXIT_FAILURE);
        }
	// Read the file, adding each integer to our array of integers (reverse order)
        while(i > -1 && fscanf(fp, "%d", &numArray[i]) != EOF) {
                i = i - 1;
        }

	// Create pack of 32 chars from array of integers
	__m256i asciiMessage  = _mm256_set_epi8((char)numArray[0], (char)numArray[1],
		(char)numArray[2], (char)numArray[3], (char)numArray[4], 
		(char)numArray[5], (char)numArray[6], (char)numArray[7],
		(char)numArray[8], (char)numArray[9], (char)numArray[10], 
		(char)numArray[11], (char)numArray[12], (char)numArray[13],
		(char)numArray[14], (char)numArray[15], (char)numArray[16],
		(char)numArray[17], (char)numArray[18], (char)numArray[19],
		(char)numArray[20], (char)numArray[21], (char)numArray[22], 
		(char)numArray[23], (char)numArray[24], (char)numArray[25],
		(char)numArray[26], (char)numArray[27], (char)numArray[28],
		(char)numArray[29], (char)numArray[30], (char)numArray[31]);
	
	// Create our pack of 32 1's (as characters)
	__m256i ones = _mm256_set1_epi8((char)1);

	// Add the ones to the asciiMessage to decode them
	__m256i decoded = _mm256_adds_epi8 (asciiMessage, ones);
	
	// Print the results
	print__m256i(decoded);	

	return 0;
}
