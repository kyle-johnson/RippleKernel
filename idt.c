////////////////////////////////////////////////
// This file is for modifying IDT entries
// it CANNOT make an IDT, only modify
// it. boiler.asm sets up an empty IDT
// at 0x600. If that is changed in boiler.asm
// then idt_ptr needs to be setup to point
// to it.
////////////////////////////////////////////////
#include <data_types.h>
#include <idt.h>

gate_desc *idt_ptr = (gate_desc *) 0x600;

void modify_gate_address(u_long ptr_to_address, u_char int_num, u_char on_off)
{
	idt_ptr[int_num].addr1 = (u_short)ptr_to_address;
	idt_ptr[int_num].addr2 = (ptr_to_address >> 16);
	if(on_off == 1)
	{
		idt_ptr[int_num].settings |= 0x8000;
	} else
	{
		idt_ptr[int_num].settings &= 0x7FFF;
	};
};

void enable_gate(u_long int_num)
{
	idt_ptr[int_num].settings |= 0x8000;
};

void disable_gate(u_long int_num)
{
	idt_ptr[int_num].settings &= 0x7FFF;
};
