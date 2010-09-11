#include <k_defines.h>

int30(regs_t *regs)
{
	char *vidmem = (char *) 0xb8000;
	switch(regs->eax)
	{
		case 0x1:
			vidmem[80*2*24] = 0xC8;
			vidmem[80*2*24+1] = 0x07;
			break;
		defualt:
			break;
	};
};
