#include <data_types.h>
#include <k_printf.h>
#include <phys_mm.h>
#include <exceptions.h>

// #define PAGE_FAULT_EXT_INFO

char *exception_types[] = { "divide error", "debug exceptions", " ",
"breakpoint", "overflow", "bounds check", "invalid opcode",
"coprocessor not available", "double fault", "coprocessor segment overrun",
"invalid TSS", "segment not present", "stack exception", "GPE",
"page fault", " ", "coprocessor error" };

void page_fault()
{
	u_long fault_address;
	u_long *tmp = (u_long *) 0x1000, *page_tabs = (u_long *) 0xFFC00000;
	u_long tmp2=0, tmp3=0;
	u_long i;
	u_char new_pt_flag=0;

	asm("pushl %eax");
	asm("movl %cr2, %eax");
	asm("movl %%eax, %w0" : "=m" (fault_address));
	asm("popl %eax");

	k_printf("\nPAGE FAULT! 0x%x\n", fault_address);
#if PAGE_FAULT_EXT_INFO
	k_printf("Extened Info... Page Dir Entry(0x%x): 0x%x\n", fault_address>>22, tmp[fault_address >> 22]);
#endif
	if((tmp[fault_address >> 22] & 1) == 0) // page directory is not present
	{
#if PAGE_FAULT_EXT_INFO
		k_printf("Modifying page directory...\n");
#endif
		tmp2 = pop_phys_mm(0x200000);	// for the page table itself
		if(tmp2 > 0)
		{
			tmp[fault_address >> 22] = tmp2 | 3;
#if PAGE_FAULT_EXT_INFO
			k_printf("info: 0x%x\n", tmp[fault_address >> 22]);
#endif

			// flush the page-translation cache now since we modified the page directory
			asm("pushl %eax");
			asm("movl %cr3, %eax");
			asm("movl %eax, %cr3");
			asm("popl %eax");

			new_pt_flag = 1; // this is as new page table
		} else
		{
			panic("System fault...  You need to restart your computer.");
			asm("cli");
			asm("hlt");	
		};
	};


	tmp3 = fault_address>>12;
#if PAGE_FAULT_EXT_INFO
	k_printf("info: 0x%x\n", page_tabs[tmp3]);
#endif

	if(new_pt_flag == 1) // if this a new page table, zero it before going on
	{
		tmp2 = tmp3;
		tmp2 &= 0xFFFFF000;
		for(i=0; i<1023; i++)
		{
			page_tabs[tmp2+i] = 0;
		};
		new_pt_flag =0;
	};

	tmp2 = pop_phys_mm(0x200000); // for a page in the page table
	if(tmp2 > 0)
	{
		page_tabs[tmp3] = tmp2;
		page_tabs[tmp3] |= 3;
	} else
	{
		panic("System fault...  You need to restart your computer.");
		asm("cli");
		asm("hlt");	
	};


	// flush the page-translation cache now since we modified the page table
	asm("pushl %eax");
	asm("movl %cr3, %eax");
	asm("movl %eax, %cr3");
	asm("popl %eax");

	outportb(0x20, 0x20);
};

void fault(u_long int_num, u_long gs, u_long fs, u_long ds, u_long es, u_long edi, u_long esi, u_long ebp, u_long esp, u_long ebx, u_long edx, u_long ecx, u_long eax, u_long error_code, u_long eip, u_long cs, u_long eflags, u_long orig_esp, u_long orig_ss) // this needs some work as really, tasks should be nuked. the only time to truely panic is if it's a ring 0 task
{
	//panic("System fault...  You need to restart your computer.");
	k_printf("\n\nException: %d(%s)... You need to restart your computer.\n\nRegs dump:\n", int_num, exception_types[int_num]);
	k_printf("eax: 0x%x ecx: 0x%x ebx: 0x%x edx: 0x%x\n", eax,  ecx,  ebx,  edx);
	k_printf("edi: 0x%x esi: 0x%x ebp: 0x%x esp: 0x%x\n", edi, esi, ebp, esp);
	k_printf("gs: 0x%x fs: 0x%x ds: 0x%x es: 0x%x\n", gs, fs, ds, es);
	k_printf("eip: 0x%x cs: 0x%x eflags: 0x%x task's esp: 0x%x task's ss: 0x%x", eip, cs, eflags, orig_esp, orig_ss);
	asm("cli");
	asm("hlt");	
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
