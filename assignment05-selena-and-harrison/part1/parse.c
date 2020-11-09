// Implement a lexer parser in this file that splits text into individual tokens.
// You may reuse any functions you write for your main shell.
// The point is that you get something small working first!
//
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_INPUTS 30

// Implement your function parse here
// Parse should accept 1 string as an argument.
// Think carefully about the return type
// --what would be a useful type to store a
// --collection of tokens?
char** parse(char str[]) {
	
	char **output = malloc(sizeof(char*) * NUM_INPUTS);	
	char * curr;
	
	int counter = 0;
	curr = strtok(str, " ");
	while (curr != NULL) {
		output[counter] = curr;
		curr = strtok (NULL, " ");
		counter++;
	}
	return output;			
}

int main(int argc, char** argv){ 
	// Call parse here with argv[1]
    	char** parsed = parse(argv[1]); 
	char** original = parsed;	
	
	while (*parsed != NULL) {
		printf("%s\n", *parsed);
		parsed++;
	}
	free(original);

	return 0;
}
