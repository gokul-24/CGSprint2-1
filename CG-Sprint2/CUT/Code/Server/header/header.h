#ifndef header
#define header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#define SIZE 1024
#define MAX_CLIENTS 5
int flag=0;
char u[SIZE];
int sock=-2;

// Receives a file name from the client
// Transfers the file data to the client

void upload(int,char*);

// Receives a file name and from the client
// Receives the file data from the client

void download(int,char*);

//Returns the ip address of the Client
char* get_ip(struct sockaddr_in,char*);

// Checks ip address of client and with the ip present in blacklist
// If a match is found 1 is returned and connection is refused.
// If no match is found 0 is returned and connection is accepted
int check_BlackList(char*);

// Receives a command - pwd or ls or cat
// Opens a pipe,runs the command 
// Transfers the output of command to client
void view(int,char* );


// Matches the username and password of client with the data from users.txt
// If a match is found the User is authenticated 
// Else if no match is found user will be treated as anonymous

const char* authenticate(int,char*);

//For every new client that connects to the server handle_client is called.
// Here the if user authenticated, the working directory will be changed 
// And the server will now communicate with the client

void handle_client(int);

// handles the interrupt signal
void handle_sigint(int);


#endif