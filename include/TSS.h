#ifndef __TSS_HEADER
#define __TSS_HEADER

#pragma pack (push, 1) // this aligns the TSSes to a byte boundary(aligning to a page boundary isn't needed)

// TSS for 386 or higher
struct TSS {
	unsigned int link;
	unsigned long esp0;
	unsigned int ss0;
	unsigned long esp1;
	unsigned int ss1;
	unsigned long esp2;
	unsigned int ss2;
	unsigned long cr3;
	unsigned int eip;
	unsigned int eflags;
	unsigned int eax;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebx;
	unsigned int esp;
	unsigned int ebp;
	unsigned int esi;
	unsigned int edi;
	unsigned int es;
	unsigned int cs;
	unsigned int ss;
	unsigned int ds;
	unsigned int fs;
	unsigned int gs;
	unsigned int ldtr;
	unsigned int trace;
	unsigned int io_map_addr;
};

#pragma pack (pop) // sets us back to the way we were

void setup_up_tsses();
void int_31(regs_t *regs);

#endif
