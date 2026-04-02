#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/time.h>
#include<sys/wait.h>


extern int isPrime(long x); // Returns zero if composite.
                            // Returns one if prime. You may use this if you want. 
int find_primes(long M)
{
 	//Your code goes in here
	int ans;
	int a1,a2;
	pid_t pid1;
	pid1=fork();
	if(pid1<0){
		perror("fork");
		exit(-1);
	}
	if(!pid1){
		pid_t pid2=fork();
		if(!pid2){
			int ans2=0;
			for(long i=M/2+1;i<=M;i++){
				ans2+=isPrime(i);
			}
			exit(ans2%256);
		}
		int ans1=0;
		for(long i=2;i<=M/2;i++){
			ans1+=isPrime(i);
		}
		int ans;
		int ch=wait(&ans);
		ans=WEXITSTATUS(ans);
		ans1+=ans;
		ans1%=256;
		exit(ans1%256);
	}
	int ch=wait(&ans);
	ans=WEXITSTATUS(ans);

	return ans; 
}
