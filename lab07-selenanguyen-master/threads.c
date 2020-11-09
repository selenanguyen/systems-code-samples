// Implement your part 2 solution here
// gcc -lpthread threads.c -o threads

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

char colors[64][64*3];
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

// Writes out the PPM file
void save() {
        FILE *fp;
        fp = fopen("threads.ppm","w+");
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

// Paint function here
void paint(int workID){
        printf("Artist %d is painting\n",workID);

        for(int i =0; i < 64*3; i++){
                colors[workID][i] = workID * i - i; // Try doing something more interesting with the colors!
        }
}

// Thread with variable arguments
void *thread(void *vargp){
        pthread_mutex_lock(&mutex1);
        paint(counter);
        counter = counter + 1;
        pthread_mutex_unlock(&mutex1);
        return NULL;
}

int main(int argc, char** argv){
	// Store our Pthread ID
        int numberOfArtists = 64; // How many child processes do we want?
	pthread_t tids[numberOfArtists];
	for(int i =0; i < numberOfArtists; ++i){
		pthread_create(&tids[i], NULL, thread, NULL);
        }
	
	for(int i=0; i < numberOfArtists; ++i){
		pthread_join(tids[i], NULL);
	}

	printf("Masterpiece(vfork.ppm) is being assembled\n");
        // Write out the PPM file       
        save();
        return 0;
}

