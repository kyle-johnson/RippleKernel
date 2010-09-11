// this file is based on John Fine's gdtnasm.inc
#ifndef DESCRIPTOR_HEADER
#define DESCRIPTOR_HEADER

// each descriptor must have one of the next 8 codes to define
// the type of descriptor
#define D_LDT		0x200	// LDT segment
#define D_TASK		0x500	// task gate
#define D_TSS		0x900	// TSS
#define D_CALL		0xC00	// 386 call gate
#define D_INT		0xE00	// 386 interrupt gate
#define D_TRAP		0xF00	// 386 trap gate
#define D_DATA		0x1000	// data segment
#define D_CODE		0x1800	// code segment

// descriptors may include the following as appropriate
#define D_DPL3		0x6000	// DPL3 or mask for DPL
#define D_DPL2		0x4000
#define D_DPL1		0x2000
#define D_PRESENT	0x8000	// present
#define D_NOT_PRESENT	0x8000

// segment descriptors(not gates) may include
#define D_ACC		0x100	// accessed(data or code)
#define D_WRITE		0x200	// writable(data segments only)
#define D_READ		0x200	// readable(code segments only)
#define D_BUSY		0x200	// busy(TSS only)
#define D_EXDOWN	0x400	// expand down(data segments only)
#define D_CONFORM	0x400	// conforming(code segments only)
#define D_BIG		0x40	// default to 32bit mode
#define D_BIG_LIM		0x80	// limit is in 4kb units

#pragma pack (push, 1)

typedef struct {
	unsigned short limit;
	unsigned short base;
	unsigned char base2;
	unsigned char type;
	unsigned char limit_and_flags;
	unsigned char base3;
} DESCRIPTOR;

#pragma pack (pop)

void make_descriptor(DESCRIPTOR *the_descriptor, u_long arg1, u_long arg2, u_long arg3);

#endif
