#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<sys/mman.h>

long how_large(void *address)
{
  /*
   *  TODO: Your code
   */ 
	long temp=(long)(address);
	temp=temp-temp%4096;
	void* up_addr=(void*)(temp+4096);
        void* down_addr=(void*)temp;
	while(1){
		void* temp_addr=mmap(up_addr, 4096,  PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
		if(temp_addr==up_addr){
			munmap(temp_addr, 4096);
			up_addr-=4096;
			break;
		}
		munmap(temp_addr, 4096);
		up_addr+=4096;
	}
	while(1){
		void* temp_addr=mmap(down_addr, 4096,  PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
		if(temp_addr==down_addr){
			munmap(temp_addr,4096);
			break;
		}
		munmap(temp_addr,4096);
		down_addr-=4096;
	}
	long up=(long)(up_addr);
	long down=(long)(down_addr);
        printf("%ld\n",(up-down));	
	return (up-down);
}
