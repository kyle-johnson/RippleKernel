#ifndef __PHYS_MM_HEADER
#define __PHYS_MM_HEADER

typedef struct {
	u_long *base;			// if this was u_huge, it could pose some problems if the stack was above 4GB(address space is limited too 32 bits still)
	u_long posistion;
	u_char mutex;
} basic_stack_struct;

void *setup_phys_mm_stack(basic_stack_struct *the_basic_stack);
void push_phys_mm(basic_stack_struct *the_stack, u_huge data);
u_huge pop_phys_mm(basic_stack_struct *the_stack);

#endif
