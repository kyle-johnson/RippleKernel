#include <data_types.h>
#include <idt.h>
#include <irqs.h>

u_short irq_watching[15] = { 0 };
u_long irq_keys[15] = {
0xAAFB39C0, 0x12984C00, 0x502EFB20, 0xB39D82C0, 0xFA29E000,
0xDCAC0210, 0xAD830610, 0x28903000, 0xB33FF000, 0x103422A0,
0xC720A290, 0xFFFFAA30, 0xD729C200, 0x00FCA320, 0x902AAF0 };

// used for un/mask_irq
u_short irqs=0;

// Installs a handler if needed to watch an IRQ.
// You must also send the right key for the number.
// this allows making sure that only one driver at
// a time is watching an IRQ and that only certified
// drivers can watch the IRQ.
// Returns 1 on success, 0 on failure.
//
// NOTE: IRQ 0 cannot be watched this way
u_char watch_irq(u_char irq_number, u_long *key)
{
	if(irq_keys[irq_number] == *key && irq_number != 0)
	{	
		irq_watching[irq_number] = 0;
		irq_keys[irq_number] += 1; // the keys table is also used to keep track of what IRQs are being currently watched
		enable_gate(irq_number+0x40);
		unmask_irq(irq_number);
		return(1);
	} else
	{
		return(0);
	};
};

// Stop watching an IRQ that was watched
// with watch_irq.
void release_irq(u_char irq_number, u_long *key)
{
	if((irq_keys[irq_number]-1) == *key && irq_number != 0)
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
