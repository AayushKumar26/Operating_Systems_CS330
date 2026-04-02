#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/mman.h>
#include <unistd.h>

int main()
{
   char *ptr;
   char *ptr1;

   ptr = mmap(NULL, 4096, PROT_READ, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
   if(ptr == MAP_FAILED){
        perror("mmap");
        exit(-1);
   }


  printf("ptr = %p\n", ptr);
  *ptr = 'x';
  printf("*ptr = %c\n", *ptr);
  return 0;
}
