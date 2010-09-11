#ifndef __PHYS_MM_HEADER
#define __PHYS_MM_HEADER

typedef struct {
	u_long *base;
	u_long posistion;
	u_long max_posistion;	// currently not used
	u_long stack_end_address;	// set by the kernel once it's done pushing pages onto the stack
	u_char mutex;
} basic_stack_struct;

void *setup_phys_mm_stack(basic_stack_struct *the_basic_stack);
void push_phys_mm(basic_stack_struct *the_stack, u_long data);
u_long pop_phys_mm(basic_stack_struct *the_stack);

#endif
