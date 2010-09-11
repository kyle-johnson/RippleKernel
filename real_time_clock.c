#include <k_defines.h>
#include <data_types.h>
#include <real_time_clock.h>

void real_time_clock_ISR(regs_t *regs)
{
	asm("cli");
	amount_of_ticks++;

	// this is needed to tell the RTC that we got the int
	outportb(0x70, 0x0C);
	(void)inportb(0x71);

	outportb(0xA0, 0x20);
	outportb(0x20, 0x20); // send an EOI for PIC1 & 2 since IRQ is on PIC2
	asm("sti");
};

// gets the time from the RTC(in 24 hour format) and puts it into strings
void get_time_str(u_char no_leading_zero, char *hr, char *min, char *sec)
{
	char num_to_char[9] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }; // for converting numbers into charactors
	u_char tmp1, tmp2;

	// getting the hour
	outportb(0x70, 0x04);
	tmp1 = inportb(0x71);
	tmp2 = tmp1 >> 4;
	tmp1 = tmp1 & 0xF;
	if(no_leading_zero == 0)
	{
		hr[0] = num_to_char[tmp2];
		hr[1] = num_to_char[tmp1];
		hr[2] = '\0';
	}
	else // cut off the leading zero(instead of "05", just have "5")
	{
		if(tmp2 != 0)
		{
			hr[0] = num_to_char[tmp2];
			hr[1] = num_to_char[tmp1];
			hr[2] = '\0';
		}
		else
		{
			hr[0] = num_to_char[tmp1];
			hr[1] = '\0';
		};
	};

	// get the minutes
	outportb(0x70, 0x02);
	tmp1 = inportb(0x71);
	tmp2 = tmp1 >> 4;
	tmp1 = tmp1 & 0xF;
	if(no_leading_zero == 0)
	{
		min[0] = num_to_char[tmp2];
		min[1] = num_to_char[tmp1];
		min[2] = '\0';
	}
	else // cut off the leading zero(instead of "05", just have "5")
	{
		if(tmp2 != 0)
		{
			min[0] = num_to_char[tmp2];
			min[1] = num_to_char[tmp1];
			min[2] = '\0';
		}
		else
		{
			min[0] = num_to_char[tmp1];
			min[1] = '\0';
		};
	};

	// get the seconds
	outportb(0x70, 0x00);
	tmp1 = inportb(0x71);
	tmp2 = tmp1 >> 4;
	tmp1 = tmp1 & 0xF;
	if(no_leading_zero == 0)
	{
		sec[0] = num_to_char[tmp2];
		sec[1] = num_to_char[tmp1];
		sec[2] = '\0';
	}
	else // cut off the leading zero(instead of "05", just have "5")
	{
		if(tmp2 != 0)
		{
			sec[0] = num_to_char[tmp2];
			sec[1] = num_to_char[tmp1];
			sec[2] = '\0';
		}
		else
		{
			sec[0] = num_to_char[tmp1];
			sec[1] = '\0';
		};
	};
};

// gets the time from the RTC(in 24 hour format) and puts it into integers
void get_time_int(int hr, int min, int sec)
{
	u_char tmp1, tmp2;

	// get hour
	outportb(0x70, 0x04);
	tmp1 = inportb(0x71);
	tmp2 = tmp1 >> 4;
	tmp1 = tmp1 & 0xF;
	tmp2 = tmp2 * 10;
	hr = tmp2 + tmp1;

	// get minutes
	outportb(0x70, 0x02);
	tmp1 = inportb(0x71);
	tmp2 = tmp1 >> 4;
	tmp1 = tmp1 & 0xF;
	tmp2 = tmp2 * 10;
	min = tmp2 + tmp1;

	// get seconds
	outportb(0x70, 0x00);
	tmp1 = inportb(0x71);
	tmp2 = tmp1 >> 4;
	tmp1 = tmp1 & 0xF;
	tmp2 = tmp2 * 10;
	sec = tmp2 + tmp1;
};

// this is temporary, eventually, the task schedular will handle this
inline void sleep(unsigned int ticks)
{
	unsigned long end_ticks;

	end_ticks = amount_of_ticks + ticks;
	while(amount_of_ticks < end_ticks);
};
