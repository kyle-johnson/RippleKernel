#include <data_types.h>
#include <k_defines.h>
#include <ports.h>
#include <mutex.h>
#include <descriptor.h>
#include <tss.h>
#include <scheduler.h>

u_char scheduler_mutex=0;
u_char flip=1;


DESCRIPTOR *the_tss_desc = (DESCRIPTOR *) 0x520;
struct TSS *the_tsses = (struct TSS *) 0x200000;

void irq_0()
{
	asm("cli");
	k_printf("\nIRQ 0\n");
	lock_mutex_block(&scheduler_mutex);

//	if(flip == 1)
//	{
//		make_tss_desc(the_tss_desc, &the_tsses[1], 108);
//		flip = 0;
//	};

	// the_tss_desc

	unlock_mutex(&scheduler_mutex);
	asm("sti");
	outportb(0x20, 0x20);
/*	asm("popl %eax");
	asm("popa");
	asm("popw %ds");
	asm("popw %es");
	asm("popw %fs");
	asm("popw %gs");
	asm("addl $8, %esp");
	asm("iret"); */
};
