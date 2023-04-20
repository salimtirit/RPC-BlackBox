/**
 * @file part_b_server.c
 * @author Salim Kemal Tirit
 
 * @brief Code for part_b the server of part_b_client, gives response to the client, executes what the client
 * wants it to execute.
 *
 * In this code, there are three pipes between parent and child. One of them works in the direction of
 * parent to child. Other two work in the direction of child to parent. These two are connected to std_out
 * and std_error respectively. This part is nearly same as part_a.
 * 
 * Both the parent and the child close the unused direction of the pipes.
 *
 * The parent sends the necessary arguments to the child process such as the path of the program to be run
 * and two integer parameters that are read from the std_in of the parent. The child runs the program with the
 * given parameters and returns the result from the compatible pipe. 
 * And the gives the result as response to client.
 *
 * How to compile and run:
    make
    ./part_b_server.out 
 */

#include "part_b.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

char **
blackbox_1_svc(numbers *argp, struct svc_req *rqstp)
{
	static char* result; //result to be returned.

	int p2c[2], c2p[2], c2pError[2], n_bytes, n_bytes_error;
	pid_t pid;
	char w_buf[10000], r_buf[10000], r_buf_Error[10000];

	if ((pipe(p2c) == -1)) {
		perror("Failed to pipe.");
		return(NULL);
	}
	if((pipe(c2p) == -1)) {
		perror("Failed to pipe.");
		return(NULL);
	}

	if((pipe(c2pError) == -1)) {
		perror("Failed to pipe.");
	}
	//three pipes created.

	if((pid=fork())== -1) {
		fprintf(stderr, "fork() failed. \n");
		exit(-1);
	}
	//fork is called

	if(pid > 0){// parent process
		close(p2c[0]); //0  read
		close(c2p[1]); //1  write
		close(c2pError[1]);
		//closing unused pipes.

		sprintf(w_buf, "%d %d", argp->a, argp->b);
		write(p2c[1], w_buf, strlen(w_buf)); //sending arguments to child.
		close(p2c[1]); //closing after using.

		//waiting to read form the pipe.
		n_bytes = read(c2p[0], r_buf, sizeof(r_buf));
		n_bytes_error = read(c2pError[0], r_buf_Error, sizeof(r_buf_Error));

		//result is being determined if it is error or not
		result = r_buf;
		if(n_bytes_error == 0) {
			result = r_buf;
		}	
		else {
			result = r_buf_Error;
		}
	}

	else {
		close(c2p[0]);
		close(p2c[1]);
		close(c2pError[0]);
		
		if((dup2(p2c[0], STDIN_FILENO) == -1) || (dup2(c2pError[1],STDERR_FILENO) == -1) || (dup2(c2p[1],STDOUT_FILENO) == -1)) {
			perror("Failed to redirect child.");
		}
		else {
			close(p2c[0]);
			close(c2p[1]);
			close(c2pError[1]);
		
			execl(argp->name,argp->name, NULL);
			perror("Failed to exec the blackbox.");
			
		}
	}

	return &result;
}
