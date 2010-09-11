#ifndef TSS_HEADER
#define TSS_HEADER

#pragma pack (push, 1) // this aligns the TSSes to a byte boundary(aligning to a page boundary isn't needed)

typedef struct {	// TSS for 386 or higher
	unsigned long link,
	esp0,
	ss0,
	esp1,
	ss1,
	esp2,
	ss2,
	cr3,
	eip,
	eflags,
	eax,
	ecx,
	edx,
	ebx,
	esp,
	ebp,
	esi,
	edi,
	es,
	cs,
	ss,
	ds,
	fs,
	gs,
	ldtr;
	unsigned int trace,
	io_map_addr;
} TSS;

#pragma pack (pop) // sets us back to the way we were


#endif
