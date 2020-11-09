// Implement your part 1 solution here
// gcc vfork.c -o vfork

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

char colors[64][64*3];

// Writes out the PPM file
void save() {
	FILE *fp;
	fp = fopen("vfork.ppm","w+");
	fputs("P3\n",fp);
	fputs("64 64\n",fp);
	fputs("255\n",fp);
	for(int i =0; i < 64;i++){
		for(int j =0; j < 64*3; j++){
			fprintf(fp,"%d",colors[i][j]);
			fputs(" ",fp);		
		}
		fputs("\n",fp);
	}
	fclose(fp);
}

// Modify your paint function here
void paint(int workID){ 
	printf("Artist %d is painting\n",workID);
	
	for(int i =0; i < 64*3; i++){
                colors[workID][i] = workID; // Try doing something more interesting with the colors!
        }
}

int main(int argc, char** argv){

        int numberOfArtists = 64; // How many child processes do we want?

	pid_t pid;
	// main loop where we fork new threads
	for(int i =0; i < numberOfArtists; i++){
                // (1) Perform a fork
        	pid = vfork();
		
		
                // (2) Make only the child do some work (i.e. paint) and then terminate.
                if(pid== 0 ){
			//  make child paint
			paint(i);
			// Then terminate/exit child
			exit(0);
		}
		printf("Child created: %d\n",pid);
	}

	// Parent
	printf("Masterpiece(vfork.ppm) is being assembled\n");
	// Write out the PPM file  	
	save();
	return 0;
}


