#ifndef header
#define header
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include<signal.h>
#include <signal.h>
#define SIZE 1024
int flag=0;



void upload(int,char*);

// Receives a file name and from the client
// Receives the file data from the client

void download(int,char*);

// Receives data from the view function written in server.c
// Called when client executes pwd/ls/cat.

void Pipe_data(int);

//Authenticates the client by communicating with the server

void auth(char*,char*,int);



#endif