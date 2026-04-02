#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/syscall.h>
extern int isPrime(long x); // Returns zero if composite.
                            // Returns one if prime. 

long find_primes(int num_cp, long M)
{
	
   /*
    *  TODO 
    *  Your code goes here
    */	
	
	long base=(M-1)/num_cp+1;
	int pin[num_cp][2];
	long ans=0;
	for(int i=0;i<num_cp;i++){
		if(syscall(SYS_pipe,pin[i])==-1){
                        perror("pipe");
                        exit(EXIT_FAILURE);
                }
	}
	for(long i=0;i<num_cp;i++){
		int pid=fork();
		if(pid<0){
			perror("fork");
			exit(-1);
		}
		if(pid==0){	// child
			long count=0;
		        for(long j=i*base;j<=base*(i+1) && j<=M;j++){
				if(j>1){
					count+=isPrime(j);
				}
			}	
			if(write(pin[i][1],&count,sizeof(long))==-1){
				perror("write");
				exit(-1);
				}
			exit(-1);	
		}
	}
	for(int i=0;i<num_cp;i++){
                long curr;
                if(read(pin[i][0],&curr,sizeof(long))==-1){
                        perror("read");
                        exit(-1);
                }
                ans+=curr;
	}
	return ans;
}













