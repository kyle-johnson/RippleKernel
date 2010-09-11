#include <k_defines.h>
#include <real_time_clock.h>

void real_time_clock_ISR(regs_t *regs)
{
	amount_of_ticks++;

	// this is needed to tell the RTC that we got the int
	outportb(0x70, 0x0C);
	(void)inportb(0x71);

	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // send an EOI for PIC1 & 2 since IRQ is on PIC2
};

void enable_real_time_clock()
{
	outportb(0x70, 0x0B);
	outportb(0x71, inportb(0x71) | 0x40);
};

void disable_real_time_clock()
{
	outportb(0x70, 0x0B);
	outportb(0x71, inportb(0x71) & 0xBF);
};

inline void sleep(unsigned int ticks)
{
	unsigned long end_ticks;

	end_ticks = amount_of_ticks + ticks;
	while(amount_of_ticks < end_ticks);
};
