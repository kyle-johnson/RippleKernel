#include <data_types.h>
#include <k_printf.h>
#include <cpu.h>

u_char identify_cpu()
{
	u_short first_eax[1];
	u_short vender_string[6];
	vender_string[6] = '\0';

	asm("pushl %eax");
	asm("pushl %ebx");
	asm("pushl %edx");
	asm("pushl %ecx");
	asm("xorl %eax, %eax");
	asm("cpuid");

	asm("movw %%ax, %w0" : "=m"(first_eax[0]));
	asm("shrl $16, %eax");
	asm("movw %%ax, %w0" : "=m"(first_eax[1]));

	asm("movw %%bx, %w0" : "=m"(vender_string[0]));
	asm("shrl $16, %ebx");
	asm("movw %%bx, %w0" : "=m"(vender_string[1]));

	asm("movw %%dx, %w0" : "=m"(vender_string[2]));
	asm("shrl $16, %edx");
	asm("movw %%dx, %w0" : "=m"(vender_string[3]));

	asm("movw %%cx, %w0" : "=m"(vender_string[4]));
	asm("shrl $16, %ecx");
	asm("movw %%cx, %w0" : "=m"(vender_string[5]));

	asm("popl %ecx");
	asm("popl %edx");
	asm("popl %ebx");
	asm("popl %eax");

	k_printf("\nVendor string: %s\n", vender_string);
	k_printf("Maximum EAX value: %x %x\n", first_eax[0], first_eax[1]);
};
