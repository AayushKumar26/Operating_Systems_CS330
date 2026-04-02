#include<tb.h>
#include<lib.h>
#include<file.h>
#include<context.h>
#include<memory.h>

/*
 * *  Trace buffer implementation.
 *
 */



///////////////////////////////////////////////////////////////////////////
////           TODO:     Trace buffer functionality                   /////
///////////////////////////////////////////////////////////////////////////


// Check whether passed buffer is valid memory location for read.
static int tb_validate(unsigned long buff, u32 count, int acflags) 
{
	struct exec_context *ctx=get_current_ctx();
	//int flag=0;
	for(int i=0;i<4;i++){
		if(i!=3 && i!=2){
			if(buff>=(ctx->mms)[i].start && buff+count<=(ctx->mms)[i].next_free-1){
				if(acflags == 1) return 67;
				return -1;
			}
		}
		else if(i == 2){
			if(buff>=(ctx->mms)[i].start && buff+count<=(ctx->mms)[i].next_free-1){
                                return 67;
                        }
		}
		else{
		       	if(buff>=(ctx->mms)[i].start && buff+count<=(ctx->mms)[i].end-1){
				return 67;
			}
		}
	}
	struct vm_area *curr=ctx->vm_area;
	while(curr!=NULL){
		if(buff>=curr->vm_start && buff+count<=curr->vm_end-1){
			if(curr->access_flags && acflags){
				return 67;
			}
			else return -1;
		}
		curr=curr->vm_next;
	}
	
      return -1;
}

static long tb_close(struct file *filep)
{	
	if(!filep){
		//printk("Error\n");
		return -1;
	}
	os_free(filep->tb->buffer,4096);
	os_free(filep->tb,sizeof(struct tb_info));
	os_free(filep,sizeof(struct file));

	
        // printk("Error\n");	
        // return -1;
}

static int tb_read(struct file *filep, char *buff, u32 count)
{	
    if(!filep || !buff)
            return -EINVAL;
    if(tb_validate((unsigned long)buff,count,2)!=67){
	    return -EBADMEM;
    }
    struct tb_info* tb_ptr = filep->tb;
    u32 wp = tb_ptr->write_off;
    u32 rp = tb_ptr->read_off;
    u32 full= tb_ptr->buff_full;
    //int written=0;
    u32 read=0;
    if(rp==wp){
	    if(full==0)	return read=0;
    }
    //u32 i=0;
    for(int i=0;i<count;i++){
	    buff[i]=tb_ptr->buffer[rp];
	    rp=(rp+1)%4096;
	    read++;
	    if(rp==wp){
		    break;
	    }
    }

    if (read>0 && full==1){
	    full=0;
    }

    tb_ptr->read_off= rp;
    tb_ptr->write_off= wp;
    tb_ptr->buff_full= full;
    return read;
}

static int tb_write(struct file *filep, char *buff, u32 count)
{
    if(!filep || !buff)
	    return -EINVAL;
    if(tb_validate((unsigned long)buff,count,1)!=67){
	   // printk("BADMEM in write\n");
	    return -EBADMEM;
    }
    struct tb_info* tb_ptr = filep->tb;
    u32 wp = tb_ptr->write_off;
    u32 rp = tb_ptr->read_off;
    u32 full= tb_ptr->buff_full;
    int written=0;
    //u32 read=0;
    if(wp==rp){
    	if(full==1)
		return written=0;
    }
   // u32 i=0;
    for(int i=0;i<count;i++){
	    tb_ptr->buffer[wp]=buff[i];
	    wp=(wp+1)%4096;
	    written++;
	    if(wp==rp){
		    full=1;
		    break;
	    }
    }


    tb_ptr->read_off= rp;
    tb_ptr->write_off= wp;
    tb_ptr->buff_full= full;
    return written;
   // printk("Error\n");	
   //return -1;
}

int sys_create_tb(struct exec_context *current, int mode)
{
    	
    int ret_fd = -1;
    //struct file* files = current->files;
	
    if(!(mode == O_READ || mode == O_WRITE || mode == O_RDWR)){
    	//printk("Error\n");
        return -EINVAL;
    }

    for(int i = 0; i < MAX_OPEN_FILES; i++){
    	if(!(current->files)[i]){
		ret_fd = i;
		break;
	}
    }

    struct file* fobj = (struct file*)os_alloc(sizeof(struct file));
    if(!fobj) return -ENOMEM;
    fobj->type = TRACE_BUFFER;
    fobj->mode = mode;
    fobj->offp = 0;
    fobj->ref_count = 1;
    fobj->inode = NULL;
    fobj->pipe= NULL;
   
    struct tb_info* tb_ptr= (struct tb_info*)os_alloc(sizeof(struct tb_info));
    if(!tb_ptr) return -ENOMEM;
    tb_ptr->buffer=(char *)os_page_alloc(USER_REG);
    tb_ptr->read_off=0;
    tb_ptr->write_off=0;
    tb_ptr->buff_full=0;
    tb_ptr->size=4096;
    fobj->tb = tb_ptr;
    

    struct fileops* fileops_ptr= (struct fileops*)(os_alloc(sizeof(struct fileops)));
    if(!fileops_ptr){
    	return -ENOMEM;
    }    
    fileops_ptr->read= tb_read;
    fileops_ptr->write= tb_write;
    fileops_ptr->close= tb_close;
    fileops_ptr->lseek= NULL;
     
    fobj->fops=fileops_ptr;
    current->files[ret_fd]=fobj;

    if(ret_fd == -1){
    	//printk("Error\n");
    	return -EINVAL;
    }
    
        

    return ret_fd;
}
