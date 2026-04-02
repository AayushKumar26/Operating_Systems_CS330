#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int gvar = 100;
int main(void)
{
	printf("%p %p\n", &main, &gvar);
	while(1)
		sleep(1);
}
