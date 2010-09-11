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
	// _memsetb(0x200000, 0, (sizeof(struct TSS)); // makes sure the mem is clear... not sure how needed this really is

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

/*	extern DESCRIPTOR extra_tss;

	DESCRIPTOR tss_1_descriptor;
	make_descriptor(&tss_1_descriptor, &our_tsses[1], 108, D_TSS);

	if(_memcmp(&tss_1_descriptor, &extra_tss, 108) != 0)
	{
		k_printf("the descriptors don't match :(\n");
	} else {
		k_printf("THE DESCRIPTORS MATCH!!!!!!!!!\n");
	};

	k_printf("%b  %b\n\n", extra_tss.limit_and_flags, extra_tss.type);
	k_printf("%b  %b\n\n", tss_1_descriptor.limit_and_flags, tss_1_descriptor.type);
*/

//	DESCRIPTOR *tss_1_descriptor = (DESCRIPTOR *) 0x520;
//	tss_1_descriptor->limit = extra_tss.limit;
//	tss_1_descriptor->base = extra_tss.base;
//	tss_1_descriptor->base2 = extra_tss.base2;
//	tss_1_descriptor->type = extra_tss.type;
//	tss_1_descriptor->limit_and_flags = extra_tss.limit_and_flags;
//	tss_1_descriptor->base3 = extra_tss.base3;
};

void int_31(regs_t *regs)
{
	DESCRIPTOR *tss_2_descriptor = (DESCRIPTOR *) 0x520;
	make_descriptor(tss_2_descriptor, &our_tsses[2], sizeof(our_tsses[2]), D_TSS);

	tss_2_descriptor->type = 0x89;

	jmp_tss_1();
};
