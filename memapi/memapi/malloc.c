#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

int main(void)
{
	char *p = malloc( 1UL << 25);
	char *q = p;
        assert(p);
	/*while(1){
		sleep(1);
	}*/
}
