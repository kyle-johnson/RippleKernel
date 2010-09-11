#ifndef __IDT_HEADER
#define __IDT_HEADER

// gate descriptor
typedef struct {
	u_short	addr1;
	u_short	selector;
	u_short	settings;
	u_short	addr2;
} gate_desc;

void modify_gate_address(u_long ptr_to_address, u_char int_num);

#endif
