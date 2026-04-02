#include <fork.h>
#include <page.h>
#include <mmap.h>
#include <apic.h>

/* #################################################*/

static inline void invlpg(unsigned long addr) {
    asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}
/**
 * cfork system call implemenations
 */

long do_cfork(){
    u32 pid;
    struct exec_context *new_ctx = get_new_ctx();
    struct exec_context *ctx = get_current_ctx();
     /* Do not modify above lines
     * 
     * */   
     /*--------------------- Your code [start]---------------*/
     
//	pid=new_ctx->pid;
	//if(!new_ctx ||
	//*new_ctx=*ctx;
	//new_ctx->ppid=ctx->pid;
	//u64 pfn=os_pfn_alloc(USER_REG);


     /*--------------------- Your code [end] ----------------*/
    
     /*
     * The remaining part must not be changed
     */
    
    copy_os_pts(ctx->pgd, new_ctx->pgd);
    do_file_fork(new_ctx);
    setup_child_context(new_ctx);
    reset_timer();

    

    return pid;
}


/* Cow fault handling, for the entire user address space
 * For address belonging to memory segments (i.e., stack, data) 
 * it is called when there is a CoW violation in these areas. 
 */

long handle_cow_fault(struct exec_context *current, u64 vaddr, int access_flags)
{
  /*long retval = -1;
	
  if(!(access_flags &2)) return  -1;

  u64* vaddr_base= (u64)* osmap(current->pgd);
  u64 l1_offset= (vaddr& PGD_MASK) >> PGD_SHIFT;
  u64 l1_addr= vaddr_base +l1_offset;
  u64 l1_content = *l1_addr;
  u64 l1_flags= l1_content  & 0xFFF;

  u64 l2_pfn= l1_content>> 12;
  u64* vaddr_base= (u64)* osmap(l2_pfn);
  u64 l2_offset= (vaddr& PUD_MASK) >> PUD_SHIFT;
  u64 l2_addr= vaddr_base +l2_offset;
  u64 l2_content = *l2_addr;
  u64 l2_flags= l2_content  & 0xFFF;

  u64 l3_pfn= l2_content>> 12;
  u64* vaddr_base= (u64)* osmap(l3_pfn);
  u64 l3_offset= (vaddr& PMD_MASK) >> PMD_SHIFT;
  u64 l3_addr= vaddr_base + l3_offset;
  u64 l3_content = *l3_addr;
  u64 l3_flags= l3_content  & 0xFFF;

  u64 l4_pfn= l3_content>> 12;
  u64* vaddr_base= (u64)* osmap(l4_pfn);
  u64 l4_offset= (vaddr& PTE_MASK) >> PTE_SHIFT;
  u64 l4_addr= vaddr_base + l4_offset;
  u64 l4_content = *l4_addr;
  u64 l4_flags = l4_content  & 0xFFF;

*/

  //return retval;
}
