#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#define SIZE 1024
int check_BlackList(char *ip){
	char cp[SIZE];
	
	FILE *fp;
	fp=fopen("../etc/ftp/client_blacklist/valid_list.txt","r");
	printf("\n");
	while(fgets(cp,SIZE,fp)!=NULL){
		cp[strlen(cp)-2]='\0';
		if(strcmp(ip,cp)==0){
			return 1;
		}
	}
	return 0;
}


const char* authenticate(char user[SIZE],char p1[SIZE]){
	int flag;
	FILE *fp;
	char dets[SIZE],name[SIZE],pass[SIZE];
	int found=0;
	fp=fopen("../data/users.TXT","r");
	while(1){
	if(fgets(dets,SIZE,fp)!=NULL){
		sscanf(dets,"%s %s",name,pass);
		if(strcmp(name,user)==0){
			//printf("\n%s  matched %s\n",user,name);
			found=1;
			break;
		}
	}
	else
		break;
	}
	if(found){
		
		if(strcmp(p1,pass)==0){
			
			
			flag=0;
			
			return "Authenticated";
		}
		else{
			
			return "Not Authenticated";
		}
	}
	else{
		
		flag=1;
		return "Authenticated";
	}
	fclose(fp);
}
