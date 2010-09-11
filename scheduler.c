#include <data_types.h>
#include <ports.h>
#include <mutex.h>
#include <descriptor.h>
#include <tss.h>
#include <k_printf.h>
#include <scheduler.h>

u_char scheduler_mutex=0;

void scheduler()
{
	lock_mutex_block(&scheduler_mutex);
	k_printf("\nIRQ 0\n");
	unlock_mutex(&scheduler_mutex);
};
