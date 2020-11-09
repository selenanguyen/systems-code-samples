// Implement your cycle count tool here.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getNumCycles(char* word);

int main(int argc, char *argv[]) {
	
	
	FILE* fptr;
	fptr  = fopen(argv[1], "r");
	if (fptr == NULL)
    	{
        	printf("%s not found.\n", *argv);
        	fprintf(stderr,"Program terminating now...");
		exit(1);
    	}
 	char buffer[255]; // At most, each line in file can be 255 characters
 	char words [255][4] = {0};
        int lineCount = 0;
	int cycles = 0;
	int instructions = 0;

	// Reads each line of the file
	while(fscanf(fptr, "%[^\n]", buffer) != EOF) {
		fgetc(fptr); // Gets next line
		sscanf(buffer, "%3s", words[lineCount]); // Puts first three characters into *words
		int numCycles = getNumCycles(words[lineCount]); // Gets the # of cycles for instruction
		if (numCycles > 0) {
			instructions++;	// If any number > 0 is returned, it is a valid instruction
		} 
		cycles += numCycles; // Add # cycles to total count
		lineCount++;
        }
	printf("Total Instructions = %d\n", instructions);
	printf("Total Cycles = %d\n", cycles);

	fclose(fptr);
	return 0;
}

// Returns the number of cycle counts given the instruction
int getNumCycles(char* word) {
	// ADD: 1, SUB: 1, MUL: 1, DIV: 19, MOV: 3, LEA: 1, PUSH: 1, POP: 1, RET: 2
	// Compares strings, ignoring case
	if(strcasecmp(word, "ADD") == 0) {
		printf("%s %d\n", "ADD", 1);
		return 1;
	}
	if(strcasecmp(word, "SUB") == 0) {
                printf("%s %d\n", "SUB", 1);
		return 1;
	}
	if(strcasecmp(word, "MUL") == 0) {
                printf("%s %d\n", "MUL", 1);
		return 1;
	}
	if(strcasecmp(word, "IMU") == 0) {
                printf("%s %d\n", "IMUL", 1);
		return 1;
	}
	if(strcasecmp(word, "DIV") == 0) {
                printf("%s %d\n", "DIV", 19);
		return 19;
	}
	if(strcasecmp(word, "MOV") == 0) {
                printf("%s %d\n", "MOV", 3);
		return 3;
	}
	if(strcasecmp(word, "LEA") == 0) {
                printf("%s %d\n", "LEA", 1);
		return 1;
	}
	if(strcasecmp(word, "PUS") == 0) {
                printf("%s %d\n", "PUSH", 1);
		return 1;
	}
	if(strcasecmp(word, "POP") == 0) {
                printf("%s %d\n", "POP", 1);
		return 1;
	}
	if(strcasecmp(word, "RET") == 0) {
                printf("%s %d\n", "RET", 2);
		return 2;
	}
	else {
		return 0;
	}
}
