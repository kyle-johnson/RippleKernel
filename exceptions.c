#include <data_types.h>
#include <k_printf.h>
#include <phys_mm.h>
#include <exceptions.h>

void fault(u_char int_num)
{
	u_long fault_address;
	u_long *tmp = (u_long *) 0x1000, *page_tabs = (u_long *) 0xFFC00000;
	u_long tmp2=0, tmp3=0;
	u_long i;

	asm("pushl %eax");
	asm("movl %cr2, %eax");
	asm("movl %%eax, %w0" : "=m" (fault_address));
	asm("popl %eax");

	k_printf("\nPAGE FAULT! 0x%x\n", fault_address);

	k_printf("Extened Info... Page Dir Entry(0x%x): 0x%x\n", fault_address>>22, tmp[fault_address >> 22]);
if(tmp[fault_address >> 22] == 2)
{
	k_printf("Modifying page directory...\n");
	tmp2 = pop_phys_mm(0x200000);	// for the page table itself
	if(tmp2 > 0)
	{
		tmp[fault_address >> 22] = tmp2;
		tmp[fault_address >> 22] |= 3;
		k_printf("info: 0x%x\n", tmp[fault_address >> 22]);

		// flush the page-translation cache now since we modified the page directory
		asm("pushl %eax");
		asm("movl %cr3, %eax");
		asm("movl %eax, %cr3");
		asm("popl %eax");

		tmp3 = fault_address>>12;
		k_printf("info: 0x%x\n", page_tabs[tmp3]);
		//for(i=0; i<1023; i++)
		//{
		//	page_tabs[i+tmp3] = 0x2;
		//};
		tmp2 = pop_phys_mm(0x200000); // for a page in the page table
		if(tmp2 > 0)
		{
			page_tabs[tmp3] = tmp2 | 3;
		} else
		{
			panic("System fault...  You need to restart your computer.");
			asm("cli");
			asm("hlt");	
		};
	} else
	{
		panic("System fault...  You need to restart your computer.");
		asm("cli");
		asm("hlt");	
	};
};
	//k_printf("Page Table Entry(0x%x): 0x%x\n\n", fault_address>>12, tmp[fault_address>>12]);
	//panic("System fault...  You need to restart your computer.");
//	asm("cli");
//	asm("hlt");

	outportb(0x20, 0x20);
};

unsigned int panic(char *message)
{
	char *vidmem = (char *) 0xb8000;

	unsigned int i=0;
	unsigned int line=0;

	i=(line*80*2);

	while(*message!=0)
	{
		vidmem[i]=*message;
		*message++;
		i++;
		vidmem[i]=0x9; // use blue text
		i++;
	};
	return(1);
};
