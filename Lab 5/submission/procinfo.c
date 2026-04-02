#include<procinfo.h>
#include<lib.h>
#include<file.h>
#include<context.h>

static char* segment_names[MAX_MM_SEGS+1] = {"code", "rodata", "data", "stack", "invalid"}; 
static char* file_types[MAX_FILE_TYPE+1] = {"stdin", "stdout", "stderr", "reg", "pipe", "invalid"}; 

long get_process_info(struct exec_context *ctx, long cmd, char *ubuf, long len)
{
    long retval = -EINVAL;	
    /*
     * TODO your code goes in here
     * */

    if(cmd == GET_PINFO_GEN){
    	if(ubuf==NULL || sizeof(struct general_info)>len){
		return retval;
	}
	struct general_info* buf=(struct general_info*) ubuf;
	buf->pid=ctx->pid;
	buf->ppid=ctx->ppid;
	buf->pcb_addr=(unsigned long)ctx;
	for(int i=0;i<CNAME_MAX;i++){
		buf->pname[i]=ctx->name[i];
	}
	return 1;
    }
    else if(cmd == GET_PINFO_FILE){
	int tot_files=0;
	for(int i=0;i<MAX_OPEN_FILES;i++){
		if(ctx->files[i]!=NULL){
			tot_files++;
		}
	}
	if(ubuf==NULL || tot_files*sizeof(struct file_info)>len){
                return retval;
        }
	struct file_info *buf = (void *) ubuf;
	int j=0;
	for(int i=0;i<MAX_OPEN_FILES;i++){
		if(ctx->files[i]!=NULL){
			buf[j].mode= ctx->files[i]->mode;
			buf[j].ref_count= ctx->files[i]->ref_count;
			buf[j].filepos=ctx->files[i]->offp;
			strcpy(buf[j].file_type,file_types[ctx->files[i]->type]);
			j++;
		}
	}
	return tot_files;	
    }
    else if(cmd == GET_PINFO_MSEG){
    	int tot_mem_seg=sizeof(ctx->mms)/sizeof(struct mm_segment);
	tot_mem_seg=(tot_mem_seg>4)?4:tot_mem_seg;
        if(ubuf==NULL || tot_mem_seg*sizeof(struct mm_segment)>len){
                return retval;
        }
        struct mem_segment_info *buf = (void *) ubuf;
        for(int i=0;i<tot_mem_seg;i++){
                buf[i].start = ctx->mms[i].start;
                buf[i].end = ctx->mms[i].end;
                buf[i].next_free = ctx->mms[i].next_free;
                strcpy(buf[i].segname,segment_names[i]);
		buf[i].perm[0]=(ctx->mms[i].access_flags & 1)?'R':'_';
		buf[i].perm[1]=(ctx->mms[i].access_flags & 2)?'W':'_';
		buf[i].perm[2]=(ctx->mms[i].access_flags & 4)?'X':'_';
        }
        return tot_mem_seg;

    }
    else if(cmd == GET_PINFO_VMA){
        struct vm_area_info *buf = (void *) ubuf;
	struct vm_area *curr=ctx->vm_area->vm_next;
	struct vm_area *temp=curr;
	int tot_areas=0;
	while(temp){
		temp=temp->vm_next;
		tot_areas++;
	}
	if(ubuf==NULL || tot_areas*sizeof(struct vm_area_info)>len){
                return retval;
        }
	int i=0;
        while(curr){
                buf[i].start = curr->vm_start;
                buf[i].end = curr->vm_end;
                buf[i].perm[0]=(curr->access_flags & 1)?'R':'_';
                buf[i].perm[1]=(curr->access_flags & 2)?'W':'_';
                buf[i].perm[2]=(curr->access_flags & 4)?'X':'_';
		curr=curr->vm_next;
		i++;
        }
        return tot_areas;
    }
    return retval;    
}
