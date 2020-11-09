// You will compile this source using the following:
// 
// clang -std=c11 -mavx2 avx4.c -o avx4prog
//
// Run as normal with: 
// 
// ./avx4prog
//
#include <stdio.h>
#include <immintrin.h>

// We can set data, load data and perform arithmetic
//
// But what if we do not exactly need to store all of the results?
// This is the idea of a 'mask'
// That is, a mask is a way to hide bits of information that are not needed
//
// Let us look at an example below.
//

// print
void print__m256(__m256 data){
	float*f = (float*)&data;
	printf("%f %f %f %f %f %f %f %f\n", f[0],f[1],f[2],f[3],f[4],f[5],f[6],f[7]);
}

void print__m256i(__m256i data){
	// 2 lines here
	int*i = (int*)&data;
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8], i[9], i[10], i[11], i[12], i[13], i[14], i[15], i[16], i[17], i[18], i[19], i[20], i[21], i[22], i[23], i[24], i[25], i[26], i[27], i[28], i[29], i[30], i[31]);
	
}

int main(){

     	// Here we are going to create a mask for integers
     	// A 'mask' is a way to hide or ignore certain data.
      	// Note in our mask, the first five values are negative, what do you think this will do?
      	// *Hint* Think about the two's complement representation of bits. The first bit indicates
      	// whether data will be shown or not--you will confirm by the end of this exercise what
      	// data is shown.
      	// We are also using the 'setr'
	__m256i mask = _mm256_setr_epi32(-1,-1,-1,-1,-1,0,2,3);
	
	// Now let us load some data from an array
	int integers[8] = {1,2,3,4,5,6,7,8};

	// Now let us load data using our mask we previously initialized.
	// Note: We are using a 'epi32' at the end, because we are storing integer results.
	__m256i result = _mm256_maskload_epi32(integers,mask);	

	// Wait so if I am using a epi32, will this print command work, or do I need another one?
	print__m256(result);

	// Here is a new print command to print of integer data. 
	print__m256i(result);

	// What do you see from the output after loading data based on a mask?
	// If I tell you a masked value with a '1' will show up, and a 0 will not
	// does that match your assumptions?
	//
	// Remember, in twos complement, what negative numbers start with!
	// Try creating a different bit mask and testing your assumptions

	// (1) Discuss: Why masking data may be important.

	return 0;
}
