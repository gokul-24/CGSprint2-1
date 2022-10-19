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
