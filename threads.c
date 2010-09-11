/////////////////////////////////////////////////////////////////////
// Threads are owned by processes. Each thread has it's own
// thread structure(thread_struct).
////////////////////////////////////////////////////////////////////
#include <data_types.h>
#include <threads.h>

thread_struct threads[1];
u_short __total_num_threads=0;

create_thread(*thread_struct new_thread, u_short process_id, u_long eip, u_long ds, u_long cs, u_long ss)
{
	u_long *stack_ptr;

	new_thread->id = __total_num_threads;
	__total_num_threads++;
	new_thread->process_id = process_id;
	new_thread->executing = 0;
	new_thread->sleeping = 0;
	stack_ptr = (u_long *) &new_thread->stack[1020];

	*stack_ptr = ss;				// ss
	stack_ptr--;
	*stack_ptr = &new_thread->stack[1023];	// esp
	stack_ptr--;

	*stack_ptr = 0x202;			// eflags
	stack_ptr--;
	*stack_ptr = cs;				// cs
	stack_ptr--;
	*stack_ptr = eip;				// eip
	stack_ptr--;

	*stack_ptr = ds;				// gs
	stack_ptr--;
	*stack_ptr = ds;				// fs
	stack_ptr--;
	*stack_ptr = ds;				// es
	stack_ptr--;
	*stack_ptr = ds;				// ds
	stack_ptr--;
	

};
