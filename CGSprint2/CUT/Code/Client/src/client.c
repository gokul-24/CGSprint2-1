#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include<signal.h>
#define SIZE 1024
int flag=0;

// Receives a file and from the client
// Transfers the file data to the client

void upload(int sockfd,char *n1){
  FILE *fp;
  fp=fopen(n1,"r");
  if(fp==NULL){
    perror("[-] Error");
	exit(1);
  }
  int n;
  char data[SIZE] = {0};

  while(1) {
	if(fgets(data, SIZE, fp) != NULL){  
		if (send(sockfd, data, sizeof(data), 0) == -1) {
			perror("[-]Error in sending file.");
			exit(1);
		}
		bzero(data, SIZE);
	}	
	else{
		send(sockfd, data,SIZE,0);
		break;
	}
  }
}

// Receives a file and from the client
// Receives the client data from the client

void download(int sockfd,char *n1){
  int n;
  FILE *fp;
  char buffer[SIZE];
  fp = fopen(n1, "w");
  
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (strlen(buffer)==0){
      break;
    }
    fputs(buffer,fp);
    bzero(buffer, SIZE);
	
  }
}

// Receives data from the view function written in server.c
// Called when client executes pwd/ls/cat.

void Pipe_data(int sockfd){
	
	char buf[SIZE];
	while(1){
			recv(sockfd, buf, sizeof(buf),0);
			if(strlen(buf)==0){
				break;
			}
			printf("%s",buf);
			bzero(buf,1024);
		  }
}

//Authenticates the client by communicating with the server

void auth(char *status,char *user,int sockfd){
	
  char msg[SIZE],auth[SIZE],pass[SIZE],p[SIZE];
  recv(sockfd,msg,SIZE,0);
  printf("%s",msg);
  scanf("%s",user);
  bzero(msg,SIZE);
  send(sockfd,user,SIZE,0);
  recv(sockfd,msg,SIZE,0);
  
  if(strcmp(msg,"Anonymous")!=0){
  flag=0;
  printf("%s:",msg);
  scanf("%s",pass);
  send(sockfd,pass,SIZE,0);
  recv(sockfd,p,SIZE,0);
  }
  else{
  strcpy(user,"Anonymous");
  strcpy(p,"Authenticated");
  flag=1;
  }
  strcpy(status,p);
}




// Creates a client socket and connects with the server
// After connection is accepted and user is authenticated a menu is displayed 
// Users can choose an option from the menu and select bye if he wants to exit

int main(){
  char *ip = "127.0.0.1";
  int port = 8021;
  int e;

  struct sockaddr_in server_addr;


  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
  
  
  char check[SIZE];
  recv(sockfd,check,SIZE,0);
  if(strcmp(check,"rejected")==0){
	  printf("server rejected connection");
	  close(sockfd);
	  return 0;
  }
  bzero(check,SIZE);
  recv(sockfd,check,SIZE,0);
  if(strcmp(check,"Max")==0){
	  printf("Max Clients reached");
	  close(sockfd);
	  return 0;
  }
	  
  printf("[+]Connected to Server.\n");
  
  
  char status[SIZE],user[SIZE];
  auth(status,user,sockfd);
  chdir("../data");
  
  if((strcmp(status,"Authenticated")==0)){
   printf("Authenticated as: %s",user);
  int ch;
  char name[100],buf[1024];
  buf[0]='\0';
  while(1){
	  
	  printf("\nEnter a choice:\n1- download - get REMOTE FILE_NAME \n2- upload - put LOCAL FILE_NAME \n3- Browse REMOTE DIRECTORY\n4- PWD\n5- Read\n6- Bye\n");
	  scanf("%d",&ch);
	  if(flag && (ch==1)){
		  printf("No Permission to execute\n");
		  continue;
	  }
	  switch(ch){
		  case 1:
		  printf("enter the file to download:");
		  scanf("%s",name);
		  strcat(buf,name);
		  strcat(buf," get");
		  send(sockfd,buf,1024,0);
		  bzero(buf,1024);
		  recv(sockfd,buf,1024,0);
		  if(strcmp(buf,"Fail")!=0){
		  download(sockfd,name);
		  printf("Downloaded Successfully\n");
		  }
		  printf("File not avaiable");
		  bzero(buf,1024);
		  break;
		  
		  case 2:
		  printf("enter the file to upload:");
		  scanf("%s",name);
		  strcat(buf,name);
		  strcat(buf," put");
		  FILE *fp;
		  fp=fopen(name,"r");
		  if(fp!=NULL){
		  send(sockfd,buf,1024,0);
		  bzero(buf,1024);
		  recv(sockfd,buf,1024,0);
		  upload(sockfd,name);
		  printf("Uploaded Successfully\n");
		  }
		  else{
			  send(sockfd,"Fail",1024,0);
			  printf("File not avaiable");
		  }
		  bzero(buf,1024);
		  break;
		  
		  case 3:
		  send(sockfd,"ls",100,0);
		  recv(sockfd, buf, sizeof(buf), 0);
		  bzero(buf,1024);
		  Pipe_data(sockfd);
		  bzero(buf,1024);
		  break;
		  
		  case 4:
		  send(sockfd,"pwd",100,0);
		  recv(sockfd, buf, sizeof(buf), 0);
		  bzero(buf,1024);
		  Pipe_data(sockfd);
		  bzero(buf,1024);
		  break;
		  
		  case 5:
		  printf("enter the file to read:");
		  scanf("%s",name);
		  
		  strcat(buf,name);
		  strcat(buf," cat");
		  send(sockfd,buf,100,0);
		  recv(sockfd, buf, sizeof(buf), 0);
		  bzero(buf,1024);

		  Pipe_data(sockfd);
		  bzero(buf,1024);
		  break;
		  
		  
		  case 6:
		  send(sockfd,"exit",100,0);
		  printf("Quitting");
		  close(sockfd);
		  exit(0);
		  break;
		  
	  }
	  
	  bzero(name,100);
	  bzero(buf,1024);
  }
  }
  else{
	  printf("Invalid Password");
  }
  close(sockfd);
  return 0;
}
