// creates a server a listents for clients to connect
// on successfull connection handles each client independently by forking
// receives operations from the clients and perfoems them successfully
#include "../header/header.h"


//Returns the ip address of the Client

char* get_ip(struct sockaddr_in new_addr,char *ip){
	sprintf(ip,"%d.%d.%d.%d",new_addr.sin_addr.s_addr & 0xff,(new_addr.sin_addr.s_addr & 0xff00) >> 8,(new_addr.sin_addr.s_addr & 0xff0000) >> 16,(new_addr.sin_addr.s_addr & 0xff000000) >> 24);
	return ip;
}

// Checks ip address of client and with the ip present in blacklist
// If a match is found 1 is returned and connection is refused.
// If no match is found 0 is returned and connection is accepted
int check_BlackList(char *ip){
	char cp[SIZE];
	
	FILE *fp;
	fp=fopen("../etc/ftp/client_blacklist/valid_list.TXT","r");
	while(fgets(cp,SIZE,fp)!=NULL){
		cp[strlen(cp)-2]='\0';
		if(strcmp(ip,cp)==0){
			return 1;
		}
	}
	return 0;
}

// Receives a file and from the client
// Transfers the file data to the client

void upload(int sockfd,char *na){
  FILE *fp;
  fp=fopen(na,"r");
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
			shutdown(sockfd, SHUT_RDWR);
			exit(1);
		}
		bzero(data, SIZE);
	}
	else{
		
		send(sockfd, data,SIZE,0);
		break;
	}
  }
  fclose(fp);
}

// Receives a file and from the client
// Receives the client data from the client

void download(int sockfd,char buf[SIZE]){
  int n;
  FILE *fp;
  char buffer[SIZE];
  if(flag==1){
	  char path[SIZE]="../Upload/";
	  strcat(path,buf);
	  strcpy(buf,path);
  }
  fp = fopen(buf, "w");
  
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
	
    if (strlen(buffer)== 0){
      break;
    }
	
    fputs(buffer,fp);
    bzero(buffer, SIZE);
  }
  fclose(fp);
  return;
}

// Receives a command - pwd or ls or cat
// Opens a pipe,runs the command 
// Transfers the output of command to client
void view(int new_sock,char *ch ){
	FILE *fp;
		char data[SIZE];
		fp=popen(ch,"r");
		while(1){
			if(fgets(data,SIZE,fp) != NULL){
				send(new_sock,data,SIZE,0);
				
				bzero(data, SIZE);
			}
			else{
				send(new_sock,data,SIZE,0);
				break;
			}
		}
		fclose(fp);
}

// Matches the username and password of client with the data from users.txt
// If a match is found the User is authenticated 
// Else if no match is found user will be treated as anonymous

const char* authenticate(int sockfd,char user[SIZE]){
	FILE *fp;
	char dets[SIZE],name[SIZE],pass[SIZE],msg[SIZE];
	int found=0;
	fp=fopen("../data/users.TXT","r");
	while(1){
	if(fgets(dets,SIZE,fp)!=NULL){
		sscanf(dets,"%s %s",name,pass);
		if(strcmp(name,user)==0){
			found=1;
			break;
		}
	}
	else
		break;
	}
	if(found){
		char p1[SIZE];
		sprintf(msg,"Welcome %s Please enter password",name);
		send(sockfd,msg,SIZE,0);
		recv(sockfd,p1,SIZE,0);
		if(strcmp(p1,pass)==0){
			send(sockfd,"Authenticated",SIZE,0);
			flag=0;
			strcpy(u,name);
			return "Authenticated";
		}
		else{
			send(sockfd,"Not Authenticated",SIZE,0);
			return "Not Authenticated";
		}
	}
	else{
		send(sockfd,"Anonymous",SIZE,0);
		strcpy(u,"pub");
		flag=1;
		return "Authenticated";
	}
	fclose(fp);
}

//For every new client that connects to the server handle_client is called.
// Here the if user authenticated, the working directory will be changed 
// And the server will now communicate with the client

void handle_client(int new_sock){
  
  char user[SIZE];
  send(new_sock,"Please enter username:",1024,0);
  recv(new_sock,user,SIZE,0);
  char val[SIZE]={0};
  strcpy(val,authenticate(new_sock,user));
  
  if(strcmp(val,"Authenticated")==0){
  char dir[SIZE]="../var/ftp/";
  if(!flag)
	  strcat(dir,u);
  else
      strcat(dir,u);
    
  chdir(dir);
	while(1){
	  char ch[SIZE],name[SIZE],case1[SIZE];
	  recv(new_sock,ch,SIZE,0);

	  if(strcmp(ch,"exit")==0){
		  break;
	  }
	  else if(strcmp(ch,"Fail")==0){
		  continue;
	  }
	  else if(strcmp(ch,"ls")==0){
		  
		send(new_sock,"Starting Pipe",SIZE,0);
	
		view(new_sock,ch);
		continue;	
	  }
	  else if(strcmp(ch,"pwd")==0){
		  
		send(new_sock,"Starting Pipe",SIZE,0);
		view(new_sock,ch);
		continue;	
	  }
	 
	  sscanf(ch,"%s %s",name,case1);
	  if(strcmp(case1,"get")==0){
		FILE *fp;
		fp=fopen(name,"r");
		if(fp!=NULL){
		send(new_sock,"Starting upload",SIZE,0);
		upload(new_sock,name);
		}
		else
		send(new_sock,"Fail",SIZE,0);
		
		
	  }
	  else if(strcmp(case1,"put")==0){
		
		send(new_sock,"Starting Download",SIZE,0);
		download(new_sock,name);
		
		
	  }
	  else if(strcmp(case1,"cat")==0){
		
		send(new_sock,"Starting Pipe",SIZE,0);
		strcat(case1," ");
		strcat(case1,name);
		
		view(new_sock,case1);
		continue;
		
	  }
	  
	  bzero(ch,SIZE);
  }
  }
  
}



// Server Socket is created, bound to port 8021 and waits for clients to join 
// For every new client a child process is created and handle_client is called
// All child processes will run independently 

int main(){

  char *ip = "127.0.0.1";
  int port = 8021;
  int e,c=0;

  int sockfd,new_sock;
  struct sockaddr_in server_addr,new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("[-]Error in bind");
    shutdown(sockfd, SHUT_RDWR);
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}
  else{
	perror("[-]Error in listening");
    shutdown(sockfd, SHUT_RDWR);
    exit(1);
	}

  
  while(1){
  int pid;
  
  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  char ip[SIZE];
  stpcpy(ip,get_ip(new_addr,ip));
  if(check_BlackList(ip)){
	  send(new_sock,"rejected",SIZE,0);
	  close(new_sock);
	  continue;
  }
  send(new_sock,"accepted",SIZE,0);
  if(c<MAX_CLIENTS){
  c++;
  
  send(new_sock,"joined",SIZE,0);
  
  if ((pid = fork()) < 0) { 
			perror("Error forking child process");
		} else if (pid == 0) { 
			close(sockfd);
			handle_client(new_sock);
			c--;
			//printf("%d",c);
			close(new_sock);
			exit(0);
		}

  }

  else{
	  send(new_sock,"Max",SIZE,0);
	  close(new_sock);
	  continue;
	  
  }
  }
 
  shutdown(sockfd, SHUT_RDWR);
  return 0;
}