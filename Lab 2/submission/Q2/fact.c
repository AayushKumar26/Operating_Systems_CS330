#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/time.h>
#include<sys/wait.h>

int main(int argc, char **argv)
{
   /*Your code goes here */
	long n=atoi(argv[1]);
	long ans;
	if(argc==3){
		ans=atoi(argv[2]);
	}
	else	
	ans=1;
	if(n==1){
		printf("%ld\n",ans);
		return 0;
	}
	if(n==2){
		printf("%ld\n",ans*2);
		return 0;
	}
	ans*=n;
	char arg1[12],arg2[20];
	sprintf(arg1,"%ld",n-1);
	sprintf(arg2,"%ld",ans);
	execl(argv[0],argv[0],arg1,arg2,NULL);
	 return 0;
}
