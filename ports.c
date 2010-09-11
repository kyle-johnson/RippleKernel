#include <ports.h>

inline void outportb(unsigned int port,unsigned char value)  // Output a byte to a port
{
	asm volatile ("outb %%al,%%dx"::"d" (port), "a" (value));
};

inline void outportw(unsigned int port,unsigned int value)  // Output a word to a port
{
	asm volatile ("outw %%ax,%%dx"::"d" (port), "a" (value));
};

inline unsigned char inportb(unsigned short port)
{
	unsigned char ret_val;

	asm volatile("inb %w1,%b0" : "=a"(ret_val) : "d"(port));
	return ret_val;
};
