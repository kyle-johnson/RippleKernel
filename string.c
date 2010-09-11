// memset, memcpy, etc
// (get permission from Frank to use these)
#include "data_types.h"

void _memsetb(void *dest, u_char c, u_long n)
{
	asm("pushl %eax");
	asm("pushl %ecx");
	asm("pushl %edi");
	asm("movl %w0, %%edi" : : "m" (dest));	// get destination pointer into edi	
	asm("movb %b0, %%al" : : "m" (c));	// get byte into al
	asm("movl %w0, %%ecx" : : "m" (n));	// get byte count into ecx
	asm("cld");
	asm("rep stosb");
	asm("popl %edi");
	asm("popl %ecx");
	asm("popl %eax");
};

void _memsetw(void *dest, u_short w, u_long n)
{
	asm("pushl %eax");
	asm("pushl %ecx");
	asm("pushl %edi");
	asm("movl %w0, %%edi" : : "m" (dest));	// get destination pointer into edi	
	asm("movw %w0, %%ax" : : "m" (w));	// get word into ax
	asm("movl %w0, %%ecx" : : "m" (n));	// get word count into ecx
	asm("cld");
	asm("rep stosw");
	asm("popl %edi");
	asm("popl %ecx");
	asm("popl %eax");
};

void _memsetd(void *dest, u_long d, u_long n)
{
	asm("pushl %eax");
	asm("pushl %ecx");
	asm("pushl %edi");
	asm("movl %w0, %%edi" : : "m" (dest));	// get destination pointer into edi	
	asm("movl %w0, %%eax" : : "m" (d));	// get dword into eax
	asm("movl %w0, %%ecx" : : "m" (n));	// get dword count into ecx
	asm("cld");
	asm("rep stosl");
	asm("popl %edi");
	asm("popl %ecx");
	asm("popl %eax");
};

void _memcpy(void *dest, void *src, u_long n)
{
	asm("pushl %ecx");
	asm("pushl %esi");
	asm("pushl %edi");
	asm("movl %w0, %%edi" : : "m" (dest));	// get destination pointer into edi
	asm("movl %w0, %%esi" : : "m" (src));	// get source pointer into esi
	asm("movl %w0, %%ecx" : : "m" (n));	// get byte count into ecx
	asm("shr $2, %ecx");	// get the dword count
	asm("cld");
	asm("rep movsl");	// blast it!
	asm("movl %w0, %%ecx" : : "m" (n));	// get byte count into ecx
	asm("andl $0x03, %ecx");	// isolate odd-byte count in ecx
	asm("cld");
	asm("rep movsb");	// write the remaining bytes
	asm("popl %edi");
	asm("popl %esi");
	asm("popl %ecx");
};
