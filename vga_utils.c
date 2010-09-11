#include <data_types.h>
#include <ports.h>
#include <string.h>
#include <vga.h>
#include <vga_utils.h>

void UnchainedClear(char color, u_short width_bytes)
{
	u_short bytes = width_bytes/4;
	u_long color4 = ((color) | (color << 8));
	color4 = ((color4) | (color4 <<  16));

	outportb(0x3c4, 2); // map mask reg
	outportb(0x3c5, 0xF); // select all planes to wirte to so we can clear them all at once

	_memsetd(0xA0000, color4, bytes);
};

void _plot_pixel(u_short x, u_short y, u_short screen_width, u_char color)
{
	u_short t, s;

	outportb(0x3c4, 2);
	s = 1;
	t = (x & 3);
	s = (s << t);
	outportb(0x3c5, s);

	// calculate the offset(inline asm time)
	asm("pushl %eax");
	asm("pushl %ebx");
	asm("pushl %edi");
	asm("xorl %eax, %eax");		// zero eax
	asm("movw %w0, %%ax" : : "m" (screen_width));
	asm("shrw $2, %ax");		// divide the width by 4
	asm("mulw %w0" : : "m" (y));		// y * (width/4)
	asm("movw %w0, %%bx" : : "m" (x));
	asm("shrw $2, %bx");		// divide 'x' by 4
	asm("addw %bx, %ax");		// we now have ((x/4)+(y*(width/4)))
	// offset calculated, onto displaying it!
	asm("movl $0xA0000, %edi");
	asm("addl %eax, %edi");
	asm("movb %b0, %%al" : : "m" (color));
	asm("movl %eax, (%edi)");		// all right, display it!
	asm("popl %edi");
	asm("popl %ebx");
	asm("popl %eax");
};
