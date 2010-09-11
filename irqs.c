#include <data_types.h>
#include <k_printf.h>
#include <idt.h>
#include <irqs.h>

volatile u_short irq_watching[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
u_long irq_keys[15] = {
0xAAFB39C0, 0x12984C00, 0x502EFB20, 0xB39D82C0, 0xFA29E000,
0xDCAC0210, 0xAD830610, 0x28903000, 0xB33FF000, 0x103422A0,
0xC720A290, 0xFFFFAA30, 0xD729C200, 0x00FCA320, 0x902AAF0 };

// used for un/mask_irq
u_short irqs=0;

// This function must be called before doing anything with IRQs.
// It remaps the PIC and sets up the ISRs for IRQs 1-15(IRQ 0
// is handled by the task scheduler).
void setup_irqs()
{
	k_printf("\nSetting up IRQs...\n");

	k_printf("Remapping the PIC... ");
	remap_pics(0x40, 0x48);
	k_printf("completed\n");

	k_printf("Masking all IRQs... ");
	mask_irq(0);
	mask_irq(1);
	mask_irq(2);
	mask_irq(3);
	mask_irq(4);
	mask_irq(5);
	mask_irq(6);
	mask_irq(7);
	mask_irq(8);
	mask_irq(9);
	mask_irq(10);
	mask_irq(11);
	mask_irq(12);
	mask_irq(13);
	mask_irq(14);
	mask_irq(15);
	k_printf("completed\n");

	k_printf("Installing ISRs for IRQs 1-15... ");
	modify_gate_address((u_long)&irq_watcher_1, 0x41, 0);
	modify_gate_address((u_long)&irq_watcher_2, 0x42, 0);
	modify_gate_address((u_long)&irq_watcher_3, 0x43, 0);
	modify_gate_address((u_long)&irq_watcher_4, 0x44, 0);
	modify_gate_address((u_long)&irq_watcher_5, 0x45, 0);
	modify_gate_address((u_long)&irq_watcher_6, 0x46, 0);
	modify_gate_address((u_long)&irq_watcher_7, 0x47, 0);
	modify_gate_address((u_long)&irq_watcher_8, 0x48, 0);
	modify_gate_address((u_long)&irq_watcher_9, 0x49, 0);
	modify_gate_address((u_long)&irq_watcher_10, 0x4A, 0);
	modify_gate_address((u_long)&irq_watcher_11, 0x4B, 0);
	modify_gate_address((u_long)&irq_watcher_12, 0x4C, 0);
	modify_gate_address((u_long)&irq_watcher_13, 0x4D, 0);
	modify_gate_address((u_long)&irq_watcher_14, 0x4E, 0);
	modify_gate_address((u_long)&irq_watcher_15, 0x4F, 0);
	k_printf("completed\n");
	k_printf("The IRQs have been setup.\n");
};

// Installs a handler if needed to watch an IRQ.
// You must also send the right key for the number.
// this allows making sure that only one driver at
// a time is watching an IRQ and that only certified
// drivers can watch the IRQ.
// Returns the address of to the variable that holds
// the number of times the IRQ has fired, 0 on failure.
//
// NOTE: IRQ 0 cannot be watched this way
u_long watch_irq(u_char irq_number, u_long key)
{
	if(irq_keys[irq_number] == key && irq_number != 0)
	{
		irq_watching[irq_number] = 0;
		irq_keys[irq_number] += 1; // the keys table is also used to keep track of what IRQs are being currently watched
		enable_gate(irq_number+0x40);
		unmask_irq(irq_number);
		return(&irq_watching[irq_number]);
	} else
	{
		return(0);
	};
};

// Stop watching an IRQ that was watched
// with watch_irq.
void release_irq(u_char irq_number, u_long key)
{
	if((irq_keys[irq_number]-1) == key && irq_number != 0)
	{
		mask_irq(irq_number); // mask the IRQ
		disable_gate(irq_number+0x40); // and disable the interrupt
		irq_keys[irq_number]--;
	};
};

// called by the IRQ interrupt handlers
void irq_watcher_helper(u_long irq_number)
{
	irq_watching[irq_number]++;
	k_printf("%d\n", irq_watching[1]);
	//putc('s');
	//inportb(0x60);
};

// Remaps the PIC, takes the interrupt numbers
// for where to remap PIC1 and PIC2 at.
void remap_pics(u_char pic1, u_char pic2)
{
	u_char a1, a2;

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

void unmask_irq(u_char irq)
{
	irqs&=~(1<<irq);
	if(irq < 8)
	{
		outportb(PIC1_DATA, irqs&0xFF);
	} else
	{
		outportb(PIC2_DATA, irqs>>8);
	};
};

void mask_irq(u_char irq)
{
	irqs|=(1<<irq);
	if(irq < 8)
	{
		outportb(PIC1_DATA, irqs&0xFF);
	} else
	{
		outportb(PIC2_DATA, irqs>>8);
	};
};
