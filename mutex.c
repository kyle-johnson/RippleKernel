/************************************************************
  Mutexes are esential. Not just on multiproccessor
  systems, but on singal porccesser systems. For
  instance, on task might malloc some memory, but
  before the memory map can be updated, another
  task mallocs some memory. Obviously, that would
  create a huge problem(probably a crash). Mutexes may
  be used to keep problems like that from happening.

	Coded: Aug. 24, 2002 by K.J.
************************************************************/
#include <data_types.h>
#include <k_printf.h>
#include <mutex.h>

u_char lock_mutex_block(u_char *mutex_pt)
{
	volatile u_char result = 1;

	while(result != 0)
	{
		// this inline assembly was taken from Frank's mutex.c file
		asm("pushl %eax");
		asm("pushl %edx");
		asm("movb $0x01, %al");
		asm("movl %w0, %%edx" : : "m" (mutex_pt));
		asm("xchgb %al, (%edx)");
		asm("movb %%al, %b0" : "=m" (result));
		asm("popl %edx");
		asm("popl %eax");
	};
};

u_char lock_mutex_noblock(u_char *mutex_pt)
{
	u_char result;

	// this inline assembly was taken from Frank's mutex.c file
	asm("pushl %eax");
	asm("pushl %edx");
	asm("movb $0x01, %al");
	asm("movl %w0, %%edx" : : "m" (mutex_pt));
	asm("xchgb %al, (%edx)");
	asm("movb %%al, %b0" : "=m" (result));
	asm("popl %edx");
	asm("popl %eax");

	return(result); // if 1, the mutex was all ready locked, if 0, we just locked it
};

void unlock_mutex(u_char *mutex_pt)
{
	*mutex_pt = 0;
};
