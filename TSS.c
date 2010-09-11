/*******************************************************
  Once this file is completed, it will be in-charge
  of dynamically changing the values in TSSes
  and saving values from them also.
*******************************************************/
#include <data_types.h>
#include <k_defines.h>
#include <tasks.h>
#include <string.h>
#include <descriptor.h>
#include <tss.h>

struct TSS *our_tsses = (struct TSS *) 0x200000;

void setup_up_tsses()
{
	our_tsses[1].cs = read_cs();
	our_tsses[1].ss = our_tsses[1].ds = our_tsses[1].es = read_ds();
	our_tsses[1].cr3 = 0x9C000;
	our_tsses[1].esp = read_esp();
	our_tsses[1].eip = (unsigned long)&task_1;

	our_tsses[2].cs = read_cs();
	our_tsses[2].ss = our_tsses[2].ds = our_tsses[2].es = read_ds();
	our_tsses[2].cr3 = 0x9C000;
	our_tsses[2].esp = read_esp();
	our_tsses[2].eip = (unsigned long)&task_2;

	// setup a descriptor
	DESCRIPTOR *tss_1_descriptor = (DESCRIPTOR *) 0x520;
	make_descriptor(tss_1_descriptor, &our_tsses[1], sizeof(our_tsses[1]), D_TSS);
	make_descriptor(&tss_1_descriptor[1], &our_tsses[2], sizeof(our_tsses[2]), D_TSS);
};

void int_31()
{
	k_printf("\nInterrupt 0x31\n");
	DESCRIPTOR *tss_2_descriptor = (DESCRIPTOR *) 0x520;
	make_descriptor(tss_2_descriptor, &our_tsses[2], sizeof(our_tsses[2]), D_TSS);

	tss_2_descriptor->type = 0x89;

	jmp_tss_1();
};
