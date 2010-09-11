#include <data_types.h>
#include <k_defines.h>
#include <ports.h>
#include <mm.h>
#include <putchar.h>
#include <floppy.h>
#include <real_time_clock.h>
#include <k_printf.h>
#include <tss.h>
#include <descriptor.h>
#include <mp.h>
#include <vga.h>
#include <vga_utils.h>
#include <idt.h>
#include <exceptions.h>
#include <cpu.h>
#include <processes.h>
#include <threads.h>
#include <tasks.h>
#include <scheduler.h>
#include <phys_mm.h>
#include <keyboard.h>
#include <irqs.h>
#include "include\malloc.h"


// video memory pointer
char *vidmem = (char *) 0xb8000;

k_main() // like main
{
	asm("cli");	// turn off ints just in case

	u_short b=0;
	u_short * a;
	u_long temp2, i;
	u_char temp;
	amount_of_ticks=0;

	k_clear_screen();

	k_printf("Now setting up the exception handlers...\n");
	modify_gate_address((u_long)&isr0, 0, 1);
	modify_gate_address((u_long)&isr1, 1, 1);
	modify_gate_address((u_long)&isr2, 2, 1);
	modify_gate_address((u_long)&isr3, 3, 1);
	modify_gate_address((u_long)&isr4, 4, 1);
	modify_gate_address((u_long)&isr5, 5, 1);
	modify_gate_address((u_long)&isr6, 6, 1);
	modify_gate_address((u_long)&isr7, 7, 1);
	modify_gate_address((u_long)&isr8, 8, 1);
	modify_gate_address((u_long)&isr9, 9, 1);
	modify_gate_address((u_long)&isr10, 10, 1);
	modify_gate_address((u_long)&isr11, 11, 1);
	modify_gate_address((u_long)&isr12, 12, 1);
	modify_gate_address((u_long)&isr13, 13, 1);
	modify_gate_address((u_long)&isr14, 14, 1);
	modify_gate_address((u_long)&isr16, 16, 1);
	k_printf("Exception handlers setup.\n");

	setup_irqs(); // do this before messing with ANYTHING that uses IRQs

//	k_printf("\nSetting up the keyboard...\n");
//	setup_keyboard();

	k_printf("\nInstalling IRQ0 handler(task switcher)...\n");
	modify_gate_address((u_long)&irq0, 0x40, 1);
	k_printf("IRQ0 handler installed.\n");

//	k_printf("Setting up the real time clock handler..\n");

//	outportb(0x70, 0x0A);
//	outportb(0x71, (inportb(0x71) | 0x06)); // set the real time clock to generate 1024 ints per second

//	enable_rtc(); // must renable the rtc since we just disabled it and will be using the sleep function

//	is_mp_present();

//	identify_cpu();

	k_printf("\n%x\n", setup_phys_mm_stack(0x200000));
	u_long *phys_page_stack = (u_long *)0x200000;
	for(temp2 = 0x300000; temp2 < 0xB00000; temp2 += 4096)
	{
		push_phys_mm(0x200000, temp2);
	};

	k_printf("Pages pushed onto the stack for the first 16MB of memory.\n");
	write_cr3(virt_mem_inti());
	k_printf("First page directory and page table setup...\nCR3 loaded with the page directory's address.\n");
	k_printf("Hold your breath, enabling paging...\n");
	enable_paging();
	k_printf("YEAH!!! Paging enabled!!!!\n");

	mm_t *mm_tmp = (mm_t *) 0x300000;

	mm_tmp->superpage_bitmap[0] = 0x0000000F;	// first 4 superpages are allocated
	for(i=4; i<1023; i++)	// set remaining 1,020 superpages to have 1,024 free pages each
	{
		mm_tmp->superpage_count[i] = 1024;
	};

	u_long *test;
	test = malloc(sizeof(u_long));
	*test = 0xAABBCCEE;
	k_printf("\n0x%x\n", *test);
	u_long *total;
	total = malloc(sizeof(u_long));
	*total = 0x11223344;
	k_printf("0x%x\n", *total);
	free(total);
	free(test);
	k_printf("memory freed!\n");


	k_printf("\nSetting up 3 tasks...\n");
	make_threads();

	k_printf("\nSwitching tasks...\n");

	cool_down_thread();

/*
	k_printf("switching to 320x240 with 256 colors...\n");

	struct Vmode curr_vmode;
	SetModeMODE_X(&curr_vmode);
	UnchainedClear(0, curr_vmode.width_bytes);

	u_short k;

	for(k=0; k<200; k++)
	{
		_plot_pixel(k, k, curr_vmode.width, 1);
	};*/

	for(;;)		// the 'idle' loop
	{
		asm("hlt");
	};
};

void k_clear_screen() // clear the entire text screen
{
	unsigned int i=0;
	while(i<(80*2*25))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
};

// coded by DF
void update_cursor(int row, int col)
{
	u_short	position=(row*80) + col;
	// cursor LOW port to vga INDEX register
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (u_char)(position&0xFF));
	// cursor HIGH port to vga INDEX register
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (u_char)((position>>8)&0xFF));
};
