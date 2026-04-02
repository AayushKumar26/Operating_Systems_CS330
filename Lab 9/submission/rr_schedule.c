#include<context.h>
#include<page.h>
#include<memory.h>
#include<lib.h>


//////////////////////  Q1: RR Scheduling   ///////////////////////////////////////
//args:
//      ctx: new exec_context to be added in the linked list
void rr_add_context(struct exec_context *ctx)
{
      /*TODO*/
	if(rr_list_head==NULL){
		rr_list_head=ctx;
		ctx->next=NULL;
		return;
	}
	struct exec_context *head=rr_list_head;
	while(head->next!=NULL){
		head=head->next;	
	}
	head->next=ctx;
	ctx->next=NULL;
       	return;
}

//args:
//      ctx: exec_context to be removed from the linked list
void rr_remove_context(struct exec_context *ctx)
{
	if(rr_list_head==ctx){
		rr_list_head=ctx->next;
		return;
	}
	struct exec_context *head=rr_list_head;
	while(head->next!=ctx){
		head=head->next;
	}
	head->next=ctx->next;
      /*TODO*/

      return;
}

//args:
//      ctx: exec_context corresponding the currently running process
struct exec_context *rr_pick_next_context(struct exec_context *ctx)
{
    /*TODO*/
	if(ctx->next!=NULL && ctx->pid!=0) 	return ctx->next;
	if(rr_list_head!=NULL)	 return rr_list_head;
     return get_ctx_by_pid(0);
}

//////////////////////  Q2: Get the PAGE TABLE details for given address   ///////////////////////////////////////


//args:
//      ctx: exec_context corresponding the currently running process
//      addr: address for which the PAGE TABLE details are to be printed

int do_walk_pt(struct exec_context *ctx, unsigned long addr)
{
    u64 *vaddr_base = (u64 *)osmap(ctx->pgd);
    /*TODO*/
	u32 l1_offset = (addr & PGD_MASK) >> PGD_SHIFT;    
	u64* l1_addr = vaddr_base + l1_offset;
	u64 l1_content = *l1_addr;
	u64 l1_paddr = (l1_content) >> 12;
	u64 l1_flag = l1_content & 0xFFF;
	if((l1_flag & 1)==0){
		printk("No L1 entry \nNo L2 entry \nNo L3 entry \nNo L4 entry\n");
		return -1;
	}
	printk("L1-entry addr: %x, L1-entry contents: %x, PFN: %x, Flags: %x\n",l1_addr,l1_content,l1_paddr,l1_flag);

	u64 *vaddr_base2 = (u64 *)osmap(l1_paddr);
	u64 l2_offset = (addr & PUD_MASK) >> PUD_SHIFT;
        u64* l2_addr = vaddr_base2 + l2_offset;
        u64 l2_content = *l2_addr;
        u64 l2_paddr = (l2_content) >> 12;
        u64 l2_flag = l2_content & 0xFFF;
	if((l2_flag & 1)==0){
                printk("No L2 entry \nNo L3 entry \nNo L4 entry\n");
		return -1;
        }
        printk("L2-entry addr: %x, L2-entry contents: %x, PFN: %x, Flags: %x\n",l2_addr,l2_content,l2_paddr,l2_flag);

	u64 *vaddr_base3 = (u64 *)osmap(l2_paddr);
	u64 l3_offset = (addr & PMD_MASK) >> PMD_SHIFT;
        u64* l3_addr = vaddr_base3 + l3_offset;
        u64 l3_content = *l3_addr;
        u64 l3_paddr = (l3_content) >> 12;
        u64 l3_flag = l3_content & 0xFFF;
	if((l3_flag & 1)==0){
                printk("No L3 entry \nNo L4 entry\n");
		return -1;
        }
        printk("L3-entry addr: %x, L3-entry contents: %x, PFN: %x, Flags: %x\n",l3_addr,l3_content,l3_paddr,l3_flag);

	u64 *vaddr_base4 = (u64*)osmap(l3_paddr);
	u64 l4_offset = (addr & PTE_MASK) >> PTE_SHIFT;
        u64* l4_addr = vaddr_base4 + l4_offset;
        u64 l4_content = *l4_addr;
        u64 l4_paddr = (l4_content) >> 12;
        u64 l4_flag = l4_content & 0xFFF;
	if((l4_flag & 1)==0){
                printk("No L4 entry\n");
		return -1;
        }
        printk("L4-entry addr: %x, L4-entry contents: %x, PFN: %x, Flags: %x\n",l4_addr,l4_content,l4_paddr,l4_flag);

    return 0;
}

