/**
 * @file part_a.c
 * @author Salim Kemal Tirit
 
 * @brief Code for part_a child process executes a given program and returns result to parent.
 *
 * In this code, there are three pipes between parent and child. One of them works in the direction of
 * parent to child. Other two work in the direction of child to parent. These two are connected to std_out
 * and std_error respectively.  
 * 
 * Both the parent and the child close the unused direction of the pipes.
 *
 * The parent sends the necessary arguments to the child process such as the path of the program to be run
 * and two integer parameters that are read from the std_in of the parent. The child runs the program with the
 * given parameters and returns the result from the compatible pipe. 
 * How to compile and run:
    make
    ./part_a.out <PATH OF THE PROGRAM> <PATH OF THE OUTPUT FILE>
    ><INTEGER_ONE>
    ><INTEGER_TWO>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[]) {
	int p2c[2], c2p[2], c2pError[2], n_bytes, n_bytes_error; 
	//one pipe: parent to child; two pipes child to parent, one for out one for error 
	
	pid_t pid;
	char w_buf[10000], r_buf[10000], r_buf_Error[10000];

	sprintf(r_buf,"\\0");
	//for the end of the string. 
	sprintf(r_buf_Error,"\\0");

	if ((pipe(p2c) == -1)) {
		perror("Failed to pipe.");
		return 1;
	}
	if((pipe(c2p) == -1)) {
		perror("Failed to pipe.");
		return 1;
	}

	if((pipe(c2pError) == -1)) {
		perror("Failed to pipe.");
	}
	/* Three pipes between child and parent */

	if((pid=fork())== -1) {
		fprintf(stderr, "fork() failed. \n");
		exit(-1);
	}

	if(pid > 0){// parent process
		close(p2c[0]); //0  read
		close(c2p[1]); //1 write
		close(c2pError[1]);

		int a, b;
		scanf("%d %d", &a, &b); //scanning for two integer parameters to give to blck box

		sprintf(w_buf, "%d %d", a, b);
		write(p2c[1], w_buf, strlen(w_buf)+1); //writing to the pipe. 
		close(p2c[1]); //close after using. 

		wait(NULL); //waiting for someting to show up.
		n_bytes = read(c2p[0], r_buf, sizeof(r_buf));
		close(c2p[0]);
		wait(NULL);
		n_bytes_error = read(c2pError[0], r_buf_Error, sizeof(r_buf_Error));
		close(c2pError[0]);

		//writing to a file whose name is taken from the arguments.
		FILE *fptr;
		fptr = fopen(argv[2], "a");

		if(fptr == NULL) {
			printf("Could not open file.\n");
			exit(EXIT_FAILURE);
		}
		//succes when error is zero
		if(n_bytes_error == 0) {
			fputs("SUCCESS:\n",fptr);
			fputs(r_buf, fptr);
		}
		//failure when error is not zero
		else {
			fputs("FAIL:\n",fptr);
			fputs(r_buf_Error, fptr);
		}
	}
	else {//child process
		close(c2p[0]);
		close(p2c[1]);
		close(c2pError[0]);
		//closing unused pipes.

		if((dup2(p2c[0], STDIN_FILENO) == -1) || (dup2(c2pError[1],STDERR_FILENO) == -1) || (dup2(c2p[1],STDOUT_FILENO) == -1)) {
			perror("Failed to redirect child.");
		}//redirection of the input output and error.
		else {
			close(c2p[1]);
			close(c2pError[1]);

			char name[10000]; //name is the path of the blackbox program
			sprintf(name, "%s", argv[1]);
		
			execl(name, name, NULL);
			perror("Failed to exec the blackbox.");
		}
	}

}