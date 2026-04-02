#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<assert.h>
#include<string.h>

#define MAX_CHARS_IN_LINE 1000
#define MAX_BUF 1000

int main(int argc, char **argv)
{
	char buf[MAX_CHARS_IN_LINE+1];
	char obuf[MAX_CHARS_IN_LINE+1];

// NOTE: Do not modify anything above this line	
/***
 *      Your Code goes here
 */	if(argc!=3){
	 	printf("Error in arg\n");
		return 0;
 	}
	int in = open(argv[1],O_RDONLY);
	int out = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666);
	if(in<0 || out<0){
		printf("Error in reading file %d %d\n",in,out);
		return 0;
	}
	int p1[2],p2[2];
	if(pipe(p1)<0||pipe(p2)<0){
		printf("Error in pipe\n");
		close(in);
		close(out);
		return 0;
	}
	pid_t pid=fork();
	if(pid<0){
		printf("Error pid\n");
                close(in);
                close(out);
                return 0;
	}
	if(pid==0){
		dup2(p1[0],STDIN_FILENO);
		dup2(p2[1],STDOUT_FILENO);
		close(p1[0]);
		close(p1[1]);
		close(p2[0]);
		close(p2[1]);
		execl("./encrypt","encrypt",NULL);
		write(1,"Error",6);
		exit(-1);
	}
	else{
		close(p1[0]);
		close(p2[1]);
		ssize_t n=0;
		char* bufline=(char*)malloc(sizeof(char)*MAX_CHARS_IN_LINE);
		while((n=read(in,bufline,MAX_CHARS_IN_LINE))>0){
			int start=0;
			for(int i=0;i<n;i++){
				if(bufline[i]=='\n' || i==n-1){
					int len=i-start+1;
					char temp[32];
					int l=sprintf(temp,"%d\n",len);
					write(p1[1],temp,l);
					write(p1[1],bufline+start,len);
					start=i+1;
				}
			}
		}
		close(p1[1]);
		close(in);
		while((n=read(p2[0],bufline,MAX_CHARS_IN_LINE))>0){
			write(out,bufline,n);
		}
		close(out);
		close(p2[0]);
		free(bufline);
	}
	return 0;
	//ssize_t n=0;
	//int pos=0;
	//while(n=read(in,buf+pos,0)<0){
	//
	//}
}

