// Implement your shell in this source file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#define BUFFER_SIZE 80
#define NUM_INPUTS 30

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
	char** history = (char**)malloc(50*sizeof(char*)); // max entries?
	int entryNum;
	int isCommandBuiltIn;
	for(entryNum = 0; entryNum < 50; entryNum++) {
		history[entryNum] = (char*) malloc(BUFFER_SIZE*sizeof(char));
	}
	int commandCount = 0;
	while(1) {
		// Boolean that determines whether the command is a built-in command
		isCommandBuiltIn = 0;
		printf("mini-shell> ");
		
		// If there is no input, break
		if(!fgets(line, BUFFER_SIZE, stdin)) {
			break;
		}

		// Remove the newline character at the end of the input
		if (line[strlen(line) - 1] == '\n') {
			line[strlen(line) - 1] = '\0';
		}

		// Exit loop if command is "exit"
		if(strcmp(line, "exit") == 0){
			break;
		}

		// Print current command history if command is "history"
		if(strcmp(line, "history") == 0) {
			isCommandBuiltIn = 1;
			int l;
			for (l = 0; l < commandCount; l++) {
				printf("%s\n", history[l]);
			}
		}
		// Add the command to the history; increment the history size by 1
		strcpy(history[commandCount], line);
		commandCount = commandCount + 1;
		
		// Represents whether the command has a pipe
		int afterPipe = 0;
		char* brgv[100] = {NULL};

		// Separates the commands
		char *commands;
		commands = strtok(line, " ");
		int i = 0;
		if (commands == NULL) {
			continue;
		}
		while (commands != NULL) {
			if (afterPipe == 0) {
				// If a pipe is detected:
				if (strcmp(commands, "|") == 0) {
					afterPipe = 1; // pipe has been found
					isCommandBuiltIn = 1; // this is a built-in command
					i = 0; // reset i to index brgv
				} else { // no pipe
					argv[i] = commands; // save commands as regular args
					i++;
				}
				commands = strtok(NULL, " ");
			} else { //afterPipe = 1
				brgv[i] = commands; // save commands as separate args
				commands = strtok(NULL, " ");
				i++;
			}
		}

		argv[i] = NULL;
		strcpy(fullPath, path);
		strcat(fullPath, argv[0]);
		// replace newlines
		for (i = 0; i < strlen(fullPath); i++) {
			if (fullPath[i] == '\n'){      
				fullPath[i] = '\0';
			}
		}
		// cd command
		if(argv[0] != NULL && strcmp(argv[0], "cd") == 0) {
			isCommandBuiltIn = 1;
			if (argv[1] == NULL) {
				printf("Error - enter a directory to change to\n");
			} else {
				chdir(argv[1]); // executes cd with argument
			}
			continue;
		}
		// help command
		if(argv[0] != NULL && strcmp(argv[0], "help") == 0) {
			isCommandBuiltIn = 1;
			printf("cd x - change directory to x\n");
			printf("help - see this help text\n");
			printf("exit - exit the mini-shell\n");
			printf("history - see all previous commands\n");
			continue;
		}

		// uses separate arg array to execute commands after pipeline
		//code adapted from the example on Piazza @214
		if (afterPipe == 1) {
			//copies /bin into the brgv[0]
			strcpy(fullPath2, path);
			strcat(fullPath2, brgv[0]);
			brgv[0] = fullPath2;
			
			
			int pipefd[2], status;
			pid_t cpid[2];
			//creates a pipe and checks if it was created properly 
			if (pipe(pipefd) == -1) {
    			perror("pipe");
    			exit(EXIT_FAILURE);
  			}

			//forks the process to use to execute the left side of the pipe
			cpid[0] = fork();
			//checks if fork was successful
			if (cpid[0] == -1) {
				perror("fork");
				exit(EXIT_FAILURE);
			}
			
			//executes left side of pipe 
			if(cpid[0]==0){
				close(pipefd[0]); //closes read side of pipe 
				dup2(pipefd[1], STDOUT_FILENO); //moves output to write side of pipe

				execve(fullPath, argv, NULL); //executes the command
				
				perror("execve0"); //handles errors
				exit(EXIT_FAILURE);
  			}

			//forks the process again to use to execute the right side of the pipe
			cpid[1] = fork();
			//checks if fork worked properly
			if (cpid[1] == -1) {
				perror("fork");
				exit(EXIT_FAILURE);
			}
			//executes right side of pipe
			if(cpid[1]==0){
				close(pipefd[1]); //closes the write side of the pipe
				dup2(pipefd[0], STDIN_FILENO); // moves input to read side of pipe
				
				execve(brgv[0],brgv,NULL); //executes the command
				
				perror("execve1"); //handle error
				exit(EXIT_FAILURE);
			}

			//close pipes
			close(pipefd[0]);
			close(pipefd[1]);

			//wait for children to exit
			waitpid(-1, &status, 0);
			waitpid(-1, &status, 0);
			continue;
		}

		//forks the process to execute normal commands
		if(fork() == 0){
			//if the command is not found (execvp return -1) then print error
			if((execvp(fullPath, argv) == -1) && isCommandBuiltIn == 0) {
				printf("Command not found--Did you mean something else?\n");
			}
			exit(1);
		} else {    
			wait(NULL);
			// child exited
		}

	}

	//frees the memory used to handle history command
	int cm;
	for ( cm = 0; cm < commandCount; cm++) {
		free(history[cm]);
	}
	//prints the exit message
	printf("mini-shell terminated\n");
	free(history);
	return 0;
}
