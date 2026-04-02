#include <stdio.h>
#include <stdlib.h>
#include "buffer_api.h"

int main(int argc, char* argv[]) {
	if(argc != 2){
		perror("usage: ./solution <testcase number>");
		return -1;
	}
	buffer_init(atoi(argv[1]));
	const struct ring_buffer *rb = buffer_get_base();

	void* db=(void*)rb->data_base;
	u64 head=rb->data_head;
	u64 tail=rb->data_tail;
	u64 size=rb->data_size;
	u64 total=0;
	u64 others=0;
	while(tail<head){
		struct perf_event_header* curr=(struct perf_event_header*)(db+(tail%size));
		if(curr->type==PERF_RECORD_SAMPLE){
			struct sample_event* samp=(struct sample_event*)(curr);
			printf("0x%lx\n",samp->addr);
		}
		else if(curr->type==PERF_RECORD_LOST){
			struct lost_event* samp1=(struct lost_event*)(curr);
			total+=samp1->lost;
		}
		else{
			others+=(curr->size);
		}
		tail+=(curr->size);
	}
	printf("number of lost records: %d\n",total);
	printf("unknown size: %d\n",others);
	return -1;
	/* ------ YOUR CODE ENDS HERE ------*/
	
	/* print formats */
	// printf("0x%...."); // to print sample addresses
	// printf("number of lost records: %....");
	// printf("unknown size: %....");

	/* ------ YOUR CODE ENDS HERE ------*/
	buffer_exit();
	return 0;
}

