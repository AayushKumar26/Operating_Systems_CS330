#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char **argv) {
	if(!argc==3){
		printf("Error");
		return -1;
	}
	char *str=argv[1];
	FILE *f=fopen(argv[2],"r");
	if(f==NULL){
		printf("Error");
		return -1;
	}
	char line[2048];
	int ans=0;
	while(fgets(line,sizeof(line),f)!=NULL){
		if(strstr(line,str)!=NULL){
			printf("FOUND\n");
			return 0;
		}
	}
	printf("NOT FOUND\n");
    /**
     * TODO: Write your code here.
     */
    return 0;
}
