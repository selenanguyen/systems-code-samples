// Implement your shell in this source file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#define BUFFER_SIZE 80
#define NUM_INPUTS 30

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

// Create a signal handler
void sigint_handler(int sig) {
	write(1,"\nmini-shell terminated\n",23); 
	exit(0);
}

int main(int argc, char* argv[100]){
	signal(SIGINT, sigint_handler);
	char line[BUFFER_SIZE];
	char fullPath[50];
	char fullPath2[50];
	char* path= "/bin/";

	while(1) {
		printf("mini-shell> ");
		if(!fgets(line, BUFFER_SIZE, stdin)) {
			break;
		}

		if (line[strlen(line) - 1] == '\n') {
			line[strlen(line) - 1] = '\0';
		}

		if(strcmp(line, "exit") == 0){
			break;
		}
		
		int afterPipe = 0;
		char* brgv[100] = { NULL };

		char *commands;
		commands = strtok(line, " ");
		int i = 0;
		if (commands == NULL) {
			continue;
		}
		while (commands != NULL) {
			if (afterPipe == 0) {
				if (strcmp(commands, "|") == 0) {
					afterPipe = 1;
					i = 0;
				} else {
					argv[i] = commands;
					i++;
				}
				commands = strtok(NULL, " ");
			} else { //afterPipe = 1
				brgv[i] = commands;
				commands = strtok(NULL, " ");
				i++;
			}
		}

		argv[i] = NULL;
		strcpy(fullPath, path);
		strcat(fullPath, argv[0]);
		for (i = 0; i < strlen(fullPath); i++) {
			if (fullPath[i] == '\n'){      
				fullPath[i] = '\0';
			}
		}
		if(argv[0] != NULL && strcmp(argv[0], "cd") == 0) {
			if (argv[1] == NULL) {
				printf("Error - enter a directory to change to\n");
			} else {
				chdir(argv[1]);
			}
			continue;
		}
		if(argv[0] != NULL && strcmp(argv[0], "help") == 0) {
			printf("cd x - change directory to x\n");
			printf("help - see this help text\n");
			printf("exit - exit the mini-shell\n");
			printf("my command (not done yet)\n");
			continue;
		}

		if (afterPipe == 1) {
			strcpy(fullPath2, path);
			strcat(fullPath2, brgv[0]);
			brgv[0] = fullPath2;
			
			int pipefd[2], status;
			pid_t cpid[2];
			if (pipe(pipefd) == -1) {
    			perror("pipe"); // print a system error message, see 'man perror'
    			exit(EXIT_FAILURE);
  			}
			cpid[0] = fork();
			if (cpid[0] == -1) { // check if fork() was successful
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if(cpid[0]==0){
				// Writing side of pipe
				// Each child gets a copy of the parent, including the created pipe
				close(pipefd[0]); // close the reading side of the in this 1st child
				dup2(pipefd[1], STDOUT_FILENO); // move output to write side of pipe
				// Execute command from child
				// if successful, execve() terminates child
				execve(fullPath, argv, NULL);
				// if not, print error and exit
				perror("execve0");
				exit(EXIT_FAILURE);
  			}
			cpid[1] = fork();
			if (cpid[1] == -1) { // check if fork() was successful
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if(cpid[1]==0){
				// Reading side of pipe
				close(pipefd[1]); // close the writing side of the pipe in 2nd child
				dup2(pipefd[0], STDIN_FILENO); // move input to read side of pipe

				if (strcmp(brgv[0], "cd") == 0) {
					printf("pipefd[0]: %s\n", pipefd[0]);
					chdir(pipefd[0]);
				} else {
					// Execute command
					// if successful, execve() terminates child
					execve(brgv[0],brgv,NULL);
					// if not, print error and exit
					perror("execve1");
					exit(EXIT_FAILURE);
				}
			}

			close(pipefd[0]); // close pipe in parent
			close(pipefd[1]); // if parent's write fd to pipe is kept open, the
		    // child will NEVER receive an EOF and thus NEVER exit

			// wait for child process to complete
			waitpid(-1, &status, 0);
			waitpid(-1, &status, 0);
			continue;
		}

		if(fork() == 0){
			if(execvp(fullPath, argv) == -1) {
                printf("Command not found--Did you mean something else?\n");
            }
			exit(1);
		} else {    
			wait(NULL);
			// child exited
		}

	}
	// command "exit" gets here, but Ctrl + C doesn't. Need to fix that
	printf("mini-shell terminated\n");
	return 0;
}
