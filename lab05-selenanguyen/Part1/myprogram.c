#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// This program executies the series of commands:
// - ls -F
// - echo "testing"
// - nl example1.c
// - cat myprogram.c
int main() {
	char* myargv[16];	// We can store up to 16 arguments.
	myargv[0]="/bin/ls";	// Our first argument is the program we want to launch.
	myargv[1]="-F";		// Any additional arguments (i.e. flags) we want to make use of.
	myargv[2]=NULL; 	// Terminate the argument list--similar to how we would terminate 
				// a character string.
				// (i.e. Set the last argument to NULL if we have no more flags.)
	myargv[3]="/bin/echo";
	myargv[4]="\"testing\"";
	myargv[5]=NULL;

	myargv[6]="/bin/nl";
        myargv[7]="example1.c";
        myargv[8]=NULL;
 	
	myargv[9]="/bin/cat";
        myargv[10]="myprogram.c";
        myargv[11]=NULL;

	int i = 0;
	while(i<10) {
		if(fork()==0){
			execve(myargv[i],&myargv[i],NULL);
			exit(1);
		}else{
			wait(NULL); // handy synchronization function again!
		}
		i += 3;
	}
	return 0;
}

