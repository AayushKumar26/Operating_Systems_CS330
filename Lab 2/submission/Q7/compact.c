#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void compact(void *start, void *end)
{

  /* 
   * TODO
   * Your code goes here
   */
	char *p1=(char*)start;
	char *p2=(char*)start;
	while(p2<((char*)end)){
		if(*p2!='\0'){
			*p1=*p2;
			p1++;
		}	
		p2++;
	}
	intptr_t del=p1-(char*)end;
	sbrk(del);
 	return;    
}
