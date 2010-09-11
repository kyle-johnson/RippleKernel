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
#include <scheduler.h>
#include <mp.h>
#include <vga.h>
#include <vga_utils.h>
#include <logo.h>

// video memory pointer
char *vidmem = (char *) 0xb8000;

void kbd_isr();

void enable_ints()
{
	asm("sti");
};
void disable_ints()
{
	asm("cli");
};

k_main() // like main
{
	unsigned int b=0;
	unsigned int * a;
	unsigned long temp2;
	vector_t v, real_clock_vector, task_jumper, scheduler_vector; // needed for keyboard ISR and int 30 ISR
	amount_of_ticks=0;
	unsigned char temp;

	k_clear_screen();
	k_load();
	k_clear_screen();

	k_printf("Right now the kernel is loaded at 1MB physical.\n\n");

	k_printf("Remapping the PIC...\n");
	remap_pics(0x40, 0x48);	// irq 0 40h irq 8 48h
	k_printf("PIC remapped starting at 40 hex.\n");

	k_printf("Enabling keyboard(IRQ 1).\n");
	unmask_irq(1);		//enable the keyboard IRQ

	k_printf("Disabling timer(IRQ 0).\n");
	mask_irq(0);		//disable the timer IRQ

	scheduler_vector.eip = (unsigned)irq_0;
	scheduler_vector.access_byte = 0x8E;

	k_printf("Changing interrupt vector for IRQ 1(keyboard)\n");
	v.eip = (unsigned)kbd_isr;
	v.access_byte = 0x8E;	// present, ring 0, '386 interrupt gate
	setvect(&v, 0x41);		// okay, DO IT!

	task_jumper.eip = (unsigned)int_31;
	task_jumper.access_byte = 0x8E;
	setvect(&task_jumper, 0x31);

	k_printf("Setting up IRQ 6(floppy...\n");
	v.eip = (unsigned)irq6;
	v.access_byte = 0x8E;	// present, ring 0, 386 interrupt gate
	setvect(&v, 0x46);
	unmask_irq(6);

	k_printf("Enabling interrupts\n");
	asm("sti");

	k_printf("Setting up 1 PD, 1 PDE, and 1024 4k pages\n");
	paging_init();
	k_printf("Done\n");

	k_printf("Turning on paging...\n");
	enable_paging();
	k_printf("Paging enabled!\n");

	k_printf("\nNow trying to malloc one 4kb page of memory...\n");
	a = (unsigned int*)real_mem_malloc();
	if(a!=-1)
	{
		k_printf("Got it!\n\n");
		k_printf("Now trying to free the memory...\n");
		real_mem_free(a);
		k_printf("Memory is free!\n\n");
	};

	k_printf("Setting up the real time clock handler..\n");
	disable_ints(); // disable interrupts while changing an interrupt handler
	real_clock_vector.eip = (unsigned)real_time_clock_ISR;
	real_clock_vector.access_byte = 0x8E;	// present, ring 0, '386 interrupt gate
	setvect(&real_clock_vector, 0x48);
	unmask_irq(8);
	enable_ints(); // renable interrupts
	k_printf("Real time clock handler has been set up and IRQ 8 has been unmasked..\n\n");

	outportb(0x70, 0x0A);
	outportb(0x71, (inportb(0x71) | 0x06)); // set the real time clock to generate 1024 ints per second

	k_printf("The real time clock will now be enabled for 6 seconds, then disabled...\n");

	enable_rtc();
//	sleep(6*1024);

	mask_irq(8);
	disable_rtc();
	k_printf("%c seconds have passed and the real time %s has been %s.\n\n", '6', "clock", "disabled");

	k_printf("Now attempting to find floppy drives...\n");
	inti_floppy();

	unmask_irq(8);
	enable_rtc(); // must renable the rtc since we just disabled it and will be using the sleep function

	if(calibrate_floppy(0) == 0)
	{
		k_printf("The first floppy drive has been calibrated!!!\n");
	}
	else
	{
		k_printf("The first floppy drive hasn't been calibrated :( \n");
	};
	char hours[3], minutes[3], seconds[3];
	get_time_str(0, &hours, &minutes, &seconds);
	k_printf("%s:%s:%s\n\n", hours, minutes, seconds);

//	setup_up_tsses();
//	load_first_ltr();
	setvect(&scheduler_vector, 0x40);
//	unmask_irq(0);
//	jmp_tss_1();

//	is_mp_present();
	k_printf("switching to 320x240 with 256 colors...\n");

	struct Vmode curr_vmode;
	SetModeMODE_X(&curr_vmode);
	UnchainedClear(0, curr_vmode.width_bytes);

	u_short x, z, k=0;

	for(z=0; z<13; z++)
	{
		for(x=0; x<19; x++)
		{
			_plot_pixel(x, z, curr_vmode.width, logo[k]);
			k++;
		};
	};

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

unsigned int k_load() // loads whatever has to be loaded
{
	unsigned int i=(80*2);
	while(i<(2*80*2)) // shows a "loading bar"
	{
		vidmem[i]=0xb2;
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};

	return(1); // no problems
};

/* by DF */
void update_cursor(int row, int col)
{
	USHORT	position=(row*80) + col;
	// cursor LOW port to vga INDEX register
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (UCHAR)(position&0xFF));
	// cursor HIGH port to vga INDEX register
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (UCHAR)((position>>8)&0xFF));
};

void remap_pics(int pic1, int pic2)
{
	UCHAR	a1, a2;

	a1=inportb(PIC1_DATA);
	a2=inportb(PIC2_DATA);

	outportb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);
	outportb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	outportb(PIC1_DATA, pic1);
	outportb(PIC2_DATA, pic2);
	outportb(PIC1_DATA, 4);
	outportb(PIC2_DATA, 2);
	outportb(PIC1_DATA, ICW4_8086);
	outportb(PIC2_DATA, ICW4_8086);
	
	outportb(PIC1_DATA, a1);
	outportb(PIC2_DATA, a2);
};

void unmask_irq(int irq)
{
	irqs&=~(1<<irq);
	    if(irq < 8)
        outportb(PIC1_DATA, irqs&0xFF);
    else
        outportb(PIC2_DATA, irqs>>8);
};

void mask_irq(int irq)
{
	irqs|=(1<<irq);

    if(irq < 8)
        outportb(PIC1_DATA, irqs&0xFF);
    else
        outportb(PIC2_DATA, irqs>>8);
};

//////////////////////////////////////////////////////////////////////////////////

void fault(regs_t *regs)
{
	k_printf("%d", regs->which_int);
	if(regs->which_int==0x40)	// timer(IRQ 0)
	{
		outportb(0x20, 0x20);
	}
	else if(regs->which_int == 0x0E)
	{
		;
	}
	else
	{
		panic("System fault...  You need to restart your computer.");
		asm("cli");
		asm("hlt");
		outportb(0x20, 0x20);
	};
};

unsigned int panic(char *message)
{
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
