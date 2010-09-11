// memset, memcpy, etc
// (I have permission from Frank to use these now)
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

u_long _strlen(u_char *s)
{
	u_long i = 0;
	while(s[i])
	{
		i++;
	};
	return i;
};

u_char _strcmp(u_char *s1, u_char *s2)
{
	u_long len1, len2, i;

	len1 = _strlen(s1);
	len2 = _strlen(s2);
	if(len1 > len2)
	{
		return 1;
	};
	else if(len1 < len2)
	{
		return -1;
	};
	else
	{
		for(i=0; i<len1; i++)
		{
			if(s1[i] > s2[i])
			{
				return 1;
			};
			else if(s1[i] < s2[i])
			{
				return -1;
			};
		};
		return 0;
	};
};

u_char _memcmp(void *m1, void *m2, u_long n)
{
	u_long i;
	u_char *_m1 = (u_char *)m1, *_m2 = (u_char *)m2;
	for(i=0; i<n; i++)
	{
		if(_m1[i] < _m2[i])
		{
			return -1;
		};
		else if(_m1[i] > _m2[i])
		{
			return 1;
		};
	};
	return 0;
};
