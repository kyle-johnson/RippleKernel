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

	// "push" 6 GP registers
	//for(i = 0; i < 7; i++)
	//{
	//	new_thread->stack[i] = 0xFFFFFFFF;
	//};

	new_thread->stack[8] = 0;				// gs
	new_thread->stack[9] = 0;				// fs
	new_thread->stack[10] = ds;			// ds
	new_thread->stack[11] = ds;			// es

	new_thread->stack[12] = 0xABCD1234;
	new_thread->stack[13] = 0x1234ABCD;
	new_thread->stack[14] = 0xABCD1234;
	new_thread->stack[15] = 0x1234ABCD;
	new_thread->stack[16] = 0xABCD1234;
	new_thread->stack[17] = 0x1234ABCD;
	new_thread->stack[18] = 0xABCD1234;

	new_thread->stack[19] = eip;			// eip
	new_thread->stack[20] = cs;			// cs
	new_thread->stack[21] = 0x200;			// eflags

	new_thread->stack[22] = &new_thread->stack[254];	// "old" esp
	new_thread->stack[23] = ss;			// ss

	new_thread->esp = &new_thread->stack[8];
};

void cool_down_thread()
{
	for(;;)
	{
		//asm("int $0x40");
		asm("hlt");
	};
};
