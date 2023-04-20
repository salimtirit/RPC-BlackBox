/**
 * @file part_b_client.c
 * @author Salim Kemal Tirit
 
 * @brief Code for part_b_client takes the arguments and calls the function in the server. Takes the result and 
 * prints to a specified file. 
 *
 * In this code, we are taking three arguments. One is the program that we want the server to run. Second is the
 * path of the output file. The third one is the ip address of the server. We pass the name of the file and the program
 * to the server. Also we take two arguments (integers) from std_in. These are the parameters that we want the server
 * to execute out program with. Than we wait for the server to respond. When the server respondes we print the response
 * to the given file. 
 * 
 * How to compile and run:
    make
    ./part_b_client.out <PATH OF THE PROGRAM> <PATH OF THE OUTPUTFILE> <IP ADDRESS OF THE SERVER>
    ><INTEGER_ONE>
    ><INTEGER_TWO>
 */

#include "part_b.h"
#include <ctype.h>

void
part_b_prog_1(char *program_name, char *path_output, char *host)
{
	CLIENT *clnt;
	numbers  blackbox_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, PART_B_PROG, PART_B_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	strcpy(blackbox_1_arg.name, program_name);
	strcpy(blackbox_1_arg.path, path_output);

	int x, y;
	scanf("%d %d", &x, &y);

	blackbox_1_arg.a = x;
	blackbox_1_arg.b = y;

	char* result = *blackbox_1(&blackbox_1_arg, clnt);
	FILE *file = fopen(path_output, "a");
	
	if (isdigit(result[0])) {
		fprintf(file, "%s\n", "SUCCESS:");
		fprintf(file, "%s", result);	
	}
	else{
		fprintf(file, "%s\n","FAIL");
		fprintf(file, "%s", result);
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host, *program_name, *path_output;

	if (argc < 4) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	program_name = argv[1];
	path_output = argv[2];
	host = argv[3];

	part_b_prog_1 (program_name, path_output, host);
exit (0);
}
