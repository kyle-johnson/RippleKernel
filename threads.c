/////////////////////////////////////////////////////////////////////
// Threads are owned by processes. Each thread has it's own
// thread structure(thread_struct).
////////////////////////////////////////////////////////////////////
#include <data_types.h>
#include <irqs.h>
#include <threads.h>

u_short __total_num_threads=0;

void create_thread(thread_struct *new_thread, u_short process_id, u_long eip, u_long ds, u_long cs, u_long ss, u_long num_parms, ...)
{
	u_char i;

	new_thread->id = __total_num_threads;
	__total_num_threads++;
	new_thread->process_id = process_id;
	new_thread->executing = 0;
	new_thread->sleeping = 0;

	new_thread->stack[240 - num_parms] = 0;				// gs
	new_thread->stack[241 - num_parms] = 0;				// fs
	new_thread->stack[242 - num_parms] = ds;				// ds
	new_thread->stack[243 - num_parms] = ds;				// es

	new_thread->stack[244 - num_parms] = 0xABCD1234;
	new_thread->stack[245 - num_parms] = 0x1234ABCD;
	new_thread->stack[246 - num_parms] = 0xABCD1234;
	new_thread->stack[247 - num_parms] = 0x1234ABCD;
	new_thread->stack[248 - num_parms] = 0xABCD1234;
	new_thread->stack[249 - num_parms] = 0x1234ABCD;
	new_thread->stack[250 - num_parms] = 0xABCD1234;

	new_thread->stack[251 - num_parms] = eip;				// eip
	new_thread->stack[252 - num_parms] = cs;				// cs
	new_thread->stack[253 - num_parms] = 0x200;			// eflags

	new_thread->stack[254 - num_parms] = &new_thread->stack[255];	// "old" esp
	new_thread->stack[255 - num_parms] = ss;				// ss

	new_thread->esp = &new_thread->stack[240 - num_parms];
};

void cool_down_thread()
{
	unmask_irq(0);
	asm("sti");
	for(;;)
	{
		//putc('m');
		asm("hlt");
	};
};
