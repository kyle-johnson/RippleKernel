#include <data_types.h>
#include <mutex.h>
#include <k_printf.h>
#include <tasks.h>
#include <threads.h>
#include <processes.h>
#include <scheduler.h>

u_char scheduler_mutex=0;

thread_struct My_Thread;
u_long new_esp;

void scheduler()
{
	lock_mutex_block(&scheduler_mutex);
	k_printf("\nIRQ 0\n");
	create_thread(&My_Thread, 0, (u_long)&task_1, 0x08, 0x10, 0x08);
	new_esp = My_Thread.esp;
	unlock_mutex(&scheduler_mutex);
};
