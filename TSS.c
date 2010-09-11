/*******************************************************
  Once this file is completed, it will be in-charge
  of dynamically changing the values in TSSes
  and saving values from them also.
*******************************************************/
#include <tss.h>
#include <tasks.h>
#include <string.h>

struct TSS *our_tsses = (struct TSS *) 0x200000;

void setup_up_tsses()
{
	// _memsetb(0x200000, 0, (sizeof(struct TSS)); // makes sure the mem is clear... not sure how needed this really is

	our_tsses[1].cs = read_cs();
	our_tsses[1].ss = our_tsses[1].ds = our_tsses[1].es = read_ds();
	our_tsses[1].cr3 = 0x9C000;
	our_tsses[1].esp = read_esp();
	our_tsses[1].eip = (unsigned long)&task_1;
};
