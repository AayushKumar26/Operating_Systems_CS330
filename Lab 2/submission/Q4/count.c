#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

int main(int argc, char **argv)
{
    /**
     * TODO: Write your code here.
     */
	if(argc!=2){
		printf("Unable to execute");
		exit(-1);
	}	
	int fd=open(argv[1],O_RDONLY);
	if(fd<0){
		printf("Unable to execute");
		exit(-1);
	}
	char buf[1];
	int idx[6];
	memset(idx,0,sizeof(idx));
	int cnt[6];
	memset(cnt,0,sizeof(cnt));
	int flag=1;
	char *str[6]={" openat("," close("," read("," write("," stat("," execve("};
       	int len[6];	
	for(int i=0;i<6;i++){
		len[i]=strlen(str[i]);
	}
	while(read(fd,buf,1)>0){
		for(int i=0;i<6;i++){
			if(idx[i]+1==len[i]){
				cnt[i]++;
				idx[i]=-1;
			}
			if(idx[i]+1>len[i]){
				idx[i]=-1;
			}	
			if(buf[0]==str[i][idx[i]+1]){
				idx[i]++;
			}
			else{
				idx[i]=-1;
			}
		}
		if(buf[0]=='\n' && flag){
			flag=0;
			memset(idx,-1,sizeof(idx));
		}
	}
	char *str1[6]={"openat","close","read","write","stat","execve"};
        for(int i=0;i<6;i++){
		printf("%s: %d\n",str1[i],cnt[i]);
        }
       	return 0;
}
