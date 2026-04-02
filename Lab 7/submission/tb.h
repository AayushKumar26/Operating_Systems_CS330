#ifndef __TB_H_
#define __TB_H_

#include <types.h>
#include <context.h>

///////////////////////////////////////////////////////////////////////
///////////////////// Trace buffer functionality /////////////////////
/////////////////////////////////////////////////////////////////////
#define TRACE_BUFFER_MAX_SIZE 4096

//Trace buffer information structure
struct tb_info
{
	char* buffer;
	u32 size ;
	u32 read_off;
	u32 write_off;
	u32 buff_full;
	//Modify as per the need
};


extern int sys_create_tb(struct exec_context *current, int mode);
#endif
