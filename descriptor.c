/***************************************
  The routine(s) in this file
  is/are based on the routine(s) from
  John Fine's gdtnasm.inc
***************************************/
#include <data_types.h>
#include <descriptor.h>

void make_descriptor(DESCRIPTOR *the_descriptor, u_long arg1, u_long arg2, u_long arg3)
{
	the_descriptor->limit = arg2;
	the_descriptor->base = arg1;
	the_descriptor->base2 = arg1 >> 16;
	the_descriptor->type = ((arg3 + D_PRESENT) >> 8);
	the_descriptor->limit_and_flags = arg3 + (arg2 >> 16);
	the_descriptor->base3 = arg1 >> 24;
};

// this is the same as the above, but optimized for TSS descriptors
void make_tss_desc(DESCRIPTOR *the_descriptor, u_long arg1, u_long arg2)
{
	the_descriptor->limit = arg2;
	the_descriptor->base = arg1;
	the_descriptor->base2 = arg1 >> 16;
	the_descriptor->type = 0x89;
	the_descriptor->limit_and_flags = 0x900 + (arg2 >> 16);
	the_descriptor->base3 = arg1 >> 24;
};
