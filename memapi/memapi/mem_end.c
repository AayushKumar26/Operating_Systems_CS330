#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char etext, edata, end; /*see man(3) end*/

int arr[1600] = {1};

int main()
{
   printf("End of text %p\n", &etext);
   printf("End of initialized data %p\n", &edata);
   printf("End of uninitialized data %p\n", &end);
   return 0;
}
  
