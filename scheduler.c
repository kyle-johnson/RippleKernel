#include <data_types.h>
#include <k_defines.h>
#include <mutex.h>
#include <scheduler.h>

u_char scheduler_mutex=0;

void irq_0()
{
	asm("cli");
	lock_mutex_block(&scheduler_mutex);


	unlock_mutex(&scheduler_mutex);
	asm("sti");
};
