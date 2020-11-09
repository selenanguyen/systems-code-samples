// For this exercise, you are going to read in data from a file called 'data.txt'
// Increment all of the values by 1 and print them to the stdout on the terminal.
// 
// You will compile this source using the following:
// clang -std=c11 -mavx2 ex1.c -o ex1prog
//
// Run as normal with: 
// ./ex1prog
//
#include <stdio.h>
#include <immintrin.h>
#include <string.h>


// print (Note, you may need to change this function or write another for your data)
void print__m256(__m256 data) {
	float*f = (float*)&data;
	printf("%f %f %f %f %f %f %f %f\n", f[0],f[1],f[2],f[3],f[4],f[5],f[6],f[7]);
}

int main(){
	// Open the file
	FILE *fp;
	int numArray[32];
	fp = fopen("./data.txt", "r");
	int i = 31;
	if(fp == NULL) {
		printf("Can't open file\n");
		exit(EXIT_FAILURE);
	}

	// Read the file, adding each integer to our array of integers
	// (Adding in reverse order so it is easier to pack when using _mm256_set_ps)
	while(i > -1 && fscanf(fp, "%d", &numArray[i]) != EOF) {
		i = i - 1;
	}

	// Split them into packed floats, each with 8 floats total
	__m256 f4 = _mm256_set_ps((float)numArray[0], (float)numArray[1], (float)numArray[2], 
		(float)numArray[3], (float)numArray[4], (float)numArray[5], 
		(float)numArray[6], (float)numArray[7]);
	__m256 f3 = _mm256_set_ps((float)numArray[8],
                (float)numArray[9], (float)numArray[10], (float)numArray[11], 
		(float)numArray[12], (float)numArray[13],
                (float)numArray[14], (float)numArray[15]);
	__m256 f2 = _mm256_set_ps((float)numArray[16], (float)numArray[17], 
		(float)numArray[18], (float)numArray[19], (float)numArray[20], 
		(float)numArray[21], (float)numArray[22], (float)numArray[23]);
	__m256 f1 = _mm256_set_ps((float)numArray[24], (float)numArray[25], 
		(float)numArray[26], (float)numArray[27], (float)numArray[28], 
		(float)numArray[29], (float)numArray[30], (float)numArray[31]);

	// Pack floats (32 1's)
	__m256 ones = _mm256_set1_ps(1.000);

	// Add each pack of floats
	__m256 f1added = _mm256_add_ps(f1, ones);	
	__m256 f2added = _mm256_add_ps(f2, ones);
	__m256 f3added = _mm256_add_ps(f3, ones);
	__m256 f4added = _mm256_add_ps(f4, ones);

	// Print the results
	print__m256(f1added);
	print__m256(f2added);
	print__m256(f3added);
	print__m256(f4added);

	fclose(fp);
	return 0;
}
