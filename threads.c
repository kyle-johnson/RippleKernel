/////////////////////////////////////////////////////////////////////
// Threads are owned by processes. Each thread has it's own
// thread structure(thread_struct).
////////////////////////////////////////////////////////////////////
#include <data_types.h>
#include <threads.h>

u_short __total_num_threads=0;

void create_thread(thread_struct *new_thread, u_short process_id, u_long eip, u_long ds, u_long cs, u_long ss)
{
	u_char i;

	new_thread->id = __total_num_threads;
	__total_num_threads++;
	new_thread->process_id = process_id;
	new_thread->executing = 0;
	new_thread->sleeping = 0;

	// "push" 7 GP registers
	for(i = 0; i < 7; i++)
	{
		new_thread->stack[i] = 0xFFFFFFFF;
	};

	new_thread->stack[8] = ds;			// ds
	new_thread->stack[9] = 0x8;			// es
	new_thread->stack[10] = 0;			// fs
	new_thread->stack[11] = 0;			// gs

	new_thread->stack[12] = eip;			// eip
	new_thread->stack[13] = cs;			// cs
	new_thread->stack[14] = 0x200;			// eflags

	new_thread->stack[15] = &new_thread->stack[254];	// "old" esp
	new_thread->stack[16] = ss;			// ss

	//new_thread->stack[3] = &new_thread->stack[4];

	new_thread->esp = &new_thread->stack[0];
};

void cool_down_thread()
{
	for(;;)
	{
		//asm("int $0x40");
		asm("hlt");
	};
};
