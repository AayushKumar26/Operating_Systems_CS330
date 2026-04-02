#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

// Use Library function such as fopen, geline, fread etc
// use manpages as mentioned in the writeup. 
// -----------------------
//
//
// Your solution begins here
int main(int argc, char *argv[]) {
	char *command = argv[1];
	char *file_name=argv[2];
	FILE *stream;
	size_t numc=0;
	size_t numw=0;
	size_t numl=0;
	char *line=NULL;
	size_t len=0;
	ssize_t nreads;
	stream=fopen(argv[2],"r");
	while((nreads=getline(&line,&len,stream))!=-1){
		numl++;
		numc+=nreads;
		for(int i=0;i<nreads;i++){
			if(line[i]!=' ' && line[i]!='\n'){
				while(i<nreads &&  line[i]!=' '){
					i++;
				}
				numw++;
			}
			else{
				
				continue;
			}
		}
		if(line[nreads-1]=='EOF'){
				numc--;
		}
	}
	if(strlen(command)!=2){
	       return 1;	
	}
	else if(command[1]=='c'){
		printf("%d\n",numc);
	}
	else if(command[1]=='w'){
                printf("%d\n",numw);
        }
	else if(command[1]=='l'){
                printf("%d\n",numl);
        }
	return 0;
}

