# RPC-BlackBox
This is a C project that implements an application using Remote Procedure Call (RPC) as the layer of communication between the client and the server. The goal is to automate the process of a binary file called "blackbox", which takes some input from the console and displays some output based on the given input values. The project has two separate parts that build on top of each other:

Part A: Running the blackbox with the given inputs, getting the output, and redirecting it to a specified file.
Part B: Upgrading the wrapper designed in Part A to be an RPC server that executes the blackbox for any client that requests.

I have created separate directories for each part of the project, which contains the source code and a Makefile for compiling the code. You can compile the code by running the 'make' command in the respective directory.


## Input & Output
To run the project, each part has its own executable file that can be run separately. 

### Part A:
To run Part A, navigate to the part_a directory and run the following command:

#### Input:
The part_a.out program takes two command-line arguments:

* blackbox: Path of the executable file for the blackbox
* part_a_output.txt: Path of the output file
 
To run the program, use the following command:

```
./part_a.out <blackbox> <output_file>

```

Replace `<blackbox>` with the path to the blackbox file, which is the executable you want to run. Replace `<output_file>` with the path to the output file, where the result of running the blackbox will be stored.

After running the program, you will be prompted to enter two integers as input for the blackbox. The main program creates a child process that executes the given blackbox. The parent process reads the input from stdin and passes it to blackbox as input. The program captures the output of the blackbox and prints it to the specified file.

#### Output:
The program will capture the output of the blackbox and write it to the specified output file (part_a_output.txt).
- If the blackbox executes successfully, the output file should contain the following text:
```
SUCCESS
<Output of the blackbox>
```
- If the blackbox fails to run, the output file should contain the following text:
```
FAIL
<Error message returned by the blackbox>
```

Note that the program will append to the end of the output file instead of overriding it.

### Part B:
To run Part B, you need to run the server and client programs separately. Open two terminal windows, navigate to the part_b directory, and run the following commands in each window:

```
./part_b_server.out
```

```
./part_b_client.out <blackbox> <output_file> <server_ip_address>
```

Replace `<server_ip_address>` with the IP address or hostname of the machine where the server is running. Replace `<blackbox>` with the path to the blackbox file, which is the executable you want to run. Replace `<output_file>` with the path to the output file, where the result of running the blackbox will be stored.

The client program calls an RPC function in the server, passing two parameters: number1, and number2. The server program is ready for clients to run the RPC function given above. This function creates a child process that executes the given blackbox and returns the response to the client. The client program then prints the response to the specified file in the following format:
- If the blackbox executes successfully, the output file should contain the following text:
```
SUCCESS:
<Output of the blackbox>
```
- If the blackbox fails to run, the output file should contain the following text:
```
FAIL:
<Error message returned by the blackbox>
```
Note that the program will append to the end of the output file instead of overriding it.




