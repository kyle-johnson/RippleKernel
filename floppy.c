#include <k_defines.h>
#include <floppy.h>

void enable_floppy_a()
{
	// this doesn't enable DMA for the floppy
	outportb(0x03F2, 0x14);
};

void reset_fdc()
{
	outportb(0x03F2, 0x0);
};
