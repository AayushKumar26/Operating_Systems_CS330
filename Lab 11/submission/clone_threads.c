#include<clone_threads.h>
#include<entry.h>
#include<context.h>
#include<memory.h>
#include<lib.h>
#include<mmap.h>
#include<fork.h>
#include<page.h>

extern int destroy_user_mappings(struct exec_context *ctx);
int debug = 0;

static void set_kstack_of_thread(struct exec_context *ctx)
{
   ctx->os_stack_pfn = os_pfn_alloc(OS_PT_REG);
   ctx->os_rsp = (((u64) ctx->os_stack_pfn) << PAGE_SHIFT) + PAGE_SIZE;
   stats->num_processes++;
   ctx->type = EXEC_CTX_USER_TH;	
}

//XXX Do not modify anything above this line

/*
  system call handler for clone, create thread like execution contexts
*/
long do_clone(void *th_func, void *user_stack, void *user_arg) 
{
	int ctr;
	struct exec_context *new_ctx = get_new_ctx();  //This is to be used for the newly created thread
	struct exec_context *ctx = get_current_ctx();
	u32 pid = new_ctx->pid;
	struct thread *n_thread;
	*new_ctx=*ctx;
        new_ctx->pid=pid;

	if(!ctx->ctx_threads){  // This is the first thread
		ctx->ctx_threads = os_alloc(sizeof(struct ctx_thread_info));
		bzero((char *)ctx->ctx_threads, sizeof(struct ctx_thread_info));
		ctx->ctx_threads->pid = ctx->pid;
	}

	/* XXX Do not modify anything above. Your implementation goes here */

	// TODO your code goes here
	for(int i=0;i<4;i++){
		if(ctx->ctx_threads->threads[i].status == TH_UNUSED){
			struct thread* new_th=os_alloc(sizeof(struct thread));
			new_th->pid=pid;
			new_th->status=TH_USED;
			new_th->parent_ctx=ctx;
			ctx->ctx_threads->threads[i]=(struct thread)(*new_th);
			break;
		}
	}
	//*new_ctx=*ctx;
	//new_ctx->pid=pid;
	new_ctx->type = EXEC_CTX_USER_TH;
	new_ctx->ctx_threads = NULL;
	new_ctx->ppid=ctx->pid;
	new_ctx->state=READY;
	new_ctx->pgd=ctx->pgd;
	//new_ctx->regs->entry_rip=th_func;

	new_ctx->mms[MM_SEG_CODE]=ctx->mms[MM_SEG_CODE];
	new_ctx->mms[MM_SEG_RODATA]=ctx->mms[MM_SEG_RODATA];
	new_ctx->mms[MM_SEG_DATA]=ctx->mms[MM_SEG_DATA];
	//struct mm_segment stack_seg= os_alloc(sizeof(struct mm_segment));
	//stack_seg.start= (unsigned long) user_stack;
	//stack_seg.end= stack_seg.start- MAX_STACK_SIZE;
	//stack_seg.next_free= (unsigned long)NULL;
	//stack_seg.access_flags=3;
	new_ctx->mms[MM_SEG_STACK]=ctx->mms[MM_SEG_STACK];

	new_ctx->vm_area=ctx->vm_area;
	new_ctx->regs.entry_rip = (u64) th_func;
	new_ctx->regs.entry_rsp = (u64) user_stack;
	new_ctx->regs.rdi= (u64) user_arg;
	new_ctx->regs.entry_cs = ctx->regs.entry_cs;
	new_ctx->regs.entry_rflags = ctx->regs.entry_rflags;
	new_ctx->regs.entry_ss = ctx->regs.entry_ss;
	for(int i=0;i<4;i++){
		new_ctx->files[i] = ctx->files[i];
	}	
	new_ctx->state=READY;

	//End of your logic

	//XXX The following two lines should be there. 

	set_kstack_of_thread(new_ctx);  //Allocate kstack for the thread
	////printk("PID %u ready to run\n", new_ctx->pid);
	return pid;
}



//handler for exit() 
/*
void do_exit(u8 normal)
{
   return;
}
*/

// XXX Reference implementation for a process exit
// You can refer this to implement your version of do_exit

void do_exit(u8 normal) 
{
  int ctr;
  struct exec_context *ctx = get_current_ctx();
  struct exec_context *new_ctx;

  if(ctx->type==EXEC_CTX_USER_TH){
  	handle_thread_exit(ctx,normal);
	if(!put_pfn(ctx->os_stack_pfn))
	os_pfn_free(OS_PT_REG, ctx->os_stack_pfn);
	release_context(ctx);
	new_ctx= pick_next_context(ctx);
	dprintk("Scheduling %s %d [ptr= %x]\n", new_ctx->name,new_ctx->pid, new_ctx);
	schedule(new_ctx);
	return;
  }

 cleanup_all_threads(ctx);
  do_file_exit(ctx);   // Cleanup the files

  // cleanup of this process
  destroy_user_mappings(ctx); 
  do_vma_exit(ctx);
  if(!put_pfn(ctx->pgd)) 
      os_pfn_free(OS_PT_REG, ctx->pgd);   //XXX Now its fine as it is a single core system
  if(!put_pfn(ctx->os_stack_pfn))
     os_pfn_free(OS_PT_REG, ctx->os_stack_pfn);
  release_context(ctx); 
  new_ctx = pick_next_context(ctx);
  dprintk("Scheduling %s:%d [ptr = %x]\n", new_ctx->name, new_ctx->pid, new_ctx); 
  schedule(new_ctx);  //Calling from exit
  return;
}



////////////////////////////////////////////////////////// Semaphore implementation ////////////////////////////////////////////////////
//
//


// A spin lock implementation using cmpxchg
// XXX you can use it for implementing the semaphore
// Do not modify this code

static void spin_init(struct spinlock *spinlock)
{
	spinlock->value = 0;
	//printk("spinlock initialised\n");
}

static void spin_lock(struct spinlock *spinlock)
{
	unsigned long *addr = &(spinlock->value);

	asm volatile(
		"mov $1,  %%rcx;"
		"mov %0,  %%rdi;"
		"try: xor %%rax, %%rax;"
		"lock cmpxchg %%rcx, (%%rdi);"
		"jnz try;"
		:
		: "r"(addr)
		: "rcx", "rdi", "rax", "memory"
	);
}

static void spin_unlock(struct spinlock *spinlock)
{
	spinlock->value = 0;
}

static int init_sem_metadata_in_context(struct exec_context *ctx)
{
   if(ctx->lock){
	   printk("Already initialized MD. Call only for the first time\n");
	   return -1;
   }
   ctx->lock = (struct lock*) os_alloc(sizeof(struct lock) * MAX_LOCKS);
   if(ctx->lock == NULL){
			printk("[pid: %d]BUG: Out of memory!\n", ctx->pid);
                        return -1;
   }
	
   for(int i=0; i<MAX_LOCKS; i++)
			ctx->lock[i].state = LOCK_UNUSED;
}

// XXX Do not modify anything above this line

/*
  system call handler for semaphore creation
*/
int do_sem_init(struct exec_context *current, sem_t *sem_id, int value)
{
	if(current->lock == NULL)
		init_sem_metadata_in_context(current);
        // TODO Your implementation goes here
	
	for(int i=0;i<MAX_LOCKS;i++){
		if(current->lock[i].state==LOCK_UNUSED){
			struct lock *t = &current->lock[i];
			t->state=LOCK_USED;
			t->sem.value=value<0?0:(unsigned long)value;
			t->sem.wait_queue=NULL;
			spin_init(&t->sem.lock);
			t->id=(u64)sem_id;
			//t->id = (u64)t;
			//*sem_id=(sem_t) t->id;
			return 0;
		}
	}
	return -EAGAIN;
}

/*
  system call handler for semaphore acquire
*/




int do_sem_wait(struct exec_context *current, sem_t *sem_id)
{
	int i;
	//if (debug) printk("pid %u in %s:%u\n", current->pid, __func__, __LINE__);
	for(i=0;i<MAX_LOCKS;i++){
		if(current->lock[i].state==LOCK_USED && current->lock[i].id== (u64)sem_id){
			break;
		}
	}
	if(i==MAX_LOCKS)	return -EAGAIN;
	struct lock* t=&current->lock[i];
	spin_lock(&t->sem.lock);
	if(t->sem.value>0){
		t->sem.value--;
		spin_unlock(&t->sem.lock);
		return 0;
	}
	current->next=NULL;
	if(!t->sem.wait_queue)	t->sem.wait_queue=current;
	else{
		struct exec_context *l=t->sem.wait_queue;
		while(l->next){
			l=l->next;
		}
		l->next=current;
	}
	//if (debug) printk("pid %u in %s:%u\n", current->pid, __func__, __LINE__);
	current->state=WAITING;
	spin_unlock(&t->sem.lock);
	schedule(pick_next_context(current));
	return 0;
}

/*
  system call handler for semaphore release
*/
int do_sem_post(struct exec_context *current, sem_t *sem_id)
{
	int i;
	//if (debug) printk("pid %u in %s:%u\n", current->pid, __func__, __LINE__);
        for(i=0;i<MAX_LOCKS;i++){
                if(current->lock[i].state==LOCK_USED && current->lock[i].id==(u64)sem_id){
                        break;
                }
        }
	if(i==MAX_LOCKS)        return -EAGAIN;
        struct lock* t=&current->lock[i];
	spin_lock(&t->sem.lock);
	if(!t->sem.wait_queue){
		t->sem.value++;
		spin_unlock(&t->sem.lock);
		return 0;
	}
	//if (debug) printk("pid %u in %s:%u\n", current->pid, __func__, __LINE__);
	struct exec_context *l=t->sem.wait_queue;
	t->sem.wait_queue = l->next;
	l->next=NULL;
	l->state=READY;
	spin_unlock(&t->sem.lock);
	return 0;
} 
