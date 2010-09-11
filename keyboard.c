#include <data_types.h>
#include <string.h>
#include <mutex.h>
#include <ports.h>
#include <idt.h>
#include <irqs.h>
#include <keyboard.h>

u_char raw_scancodes[255];	// 256 char buffer for scancodes
u_char curr_scancode_pos=0;	// keep track of where we are
u_char curr_scancode_retrieve=0;
u_char keyboard_mutex=0;

void write_kbd(u_char adr, u_char data)
{
	u_long timeout;
	u_char stat;

	for(timeout = 500000; timeout != 0; timeout--)
	{
		stat = inportb(0x64);
		if((stat & 0x02) == 0) // loop until 8042 input buffer empty
		{
			break;
		};
	};
	if(timeout == 0)
	{
		k_printf("write_kbd: timeout\n");
		return;
	};
	outportb(adr, data);
};

// 0 = failure
u_char read_kbd()
{
	u_long timeout;
	u_char stat, data;

	for(timeout = 500000; timeout != 0; timeout--)
	{
		stat = inportb(0x64);
		if(stat & 0x01) // loop until 8042 output buffer full
		{
			data = inportb(0x60);
			if((stat & 0xC0) == 0) // loop if parity error or receive timeout
			{
				return data;
			};
		};
	};
	return 0;
};

// 1 = succes
u_char write_kbd_await_ack(u_char val)
{
	u_char got;

	write_kbd(0x60, val);
	got = read_kbd();
	if(got != 0xFA)
	{
		k_printf("write_kbd_await_ack: expected acknowledge (0xFA), got 0x%x\n", got);
		return 0;
	};
	return 1;
};

/********************************************************
 This identifies the current keyboard as being MF II, then checks the
 current scancode set and changes it to set 2 if it is not currently set 2.

 NOTE: work on keyboard driver so that it doesn't require cli/sti, but
 just mutexes.
********************************************************/
void setup_keyboard()
{
	volatile u_short *keyboard_isr_fired;

	// first, find out what kinda keyboard we have here
	k_printf("Detecting type of keyboard...\n");

	switch(kbd_get_id())
	{
		case 0:
			k_printf("Unknown keyboard... Ripple has halted.\n");
			asm("cli");
			asm("hlt");
		case 1:
			k_printf("AT keyboard found, Ripple requires a newer keyboard... Ripple has halted.\n");
			asm("cli");
			asm("hlt");
		case 2:
			k_printf("MF II keyboard detected.\n");
			break;
		default:
			k_printf("No keyboard found, Ripple requires a keyboard... Ripple has halted.\n");
			asm("cli");
			asm("hlt");
	};

	asm("cli");

	// setup scancode set 2
	kbd_set_scancode_set(2);
	k_printf("Scancode set is: %d\n", kbd_curr_scancode_set());

	// disable "IBM PC compatibility mode"(no converting scancode sets to set 1)
	write_kbd(0x64, 0x60);
	write_kbd(0x60, 0x07);

	k_printf("\nWatching IRQ 1... \n");
	keyboard_isr_fired = watch_irq(1, 0x12984C00);
	if(keyboard_isr_fired != 0)
	{
		*keyboard_isr_fired = 0;
		k_printf("success, IRQ has been allocated.\n");
		asm("sti");
		for(;;)
		{
			//putc('x');
			if(*keyboard_isr_fired >= 1)
			{
				k_printf("%x\n", inportb(0x60));
				*keyboard_isr_fired -= 1;
			};
		};
	} else
	{
		asm("sti");
		k_printf("failure\n");
		return;
	};
};

void store_scancode()
{
	//lock_mutex_block(&keyboard_mutex);
	if(curr_scancode_pos != 256)
	{
		raw_scancodes[curr_scancode_pos] = inportb(0x60);
		curr_scancode_pos++;	
	};
	//keyboard_mutex = 0;
};

u_char get_raw_scancode()
{
	return(0);
};

// checks to see if a char is waiting to be read from
// the buffer, returns 1 if one is, 0 if one isn't
u_char DataInKbdBuffer()
{
	u_short i;
	u_char tmp;
	for(i=0; i < 0xFFFF; i++)	// waiting loop
	{
		tmp = inportb(0x64);
		if((tmp & 0x01) == 0x01)	// is a char waiting in the buffer?
		{
			return(1);
		};
	};
	return(0);
};

u_char kbd_get_id()
{
	u_char ret_code;

	outportb(0x60, 0xf2);		// identify keyboard command
	if(!DataInKbdBuffer())
	{
		ret_code = 0;	// XT-keyboard or unknown keyboard
	} else
	{
		if(inportb(0x60) != 0xFA)
		{
			ret_code = 4;	// no ACK, we have an error

		} else
		{
			if(!DataInKbdBuffer())
			{
				ret_code = 1;	// ACK only, no ID byte so this is just a plain ol' AT keyboard
			} else
			{
				if(inportb(0x60) != 0xAB)
				{
					ret_code = 5;	// ID byte isn't for an MF II
				} else
				{
					if(!DataInKbdBuffer())
					{
						ret_code = 6;	// no second ID byte, error
					} else
					{
						if(inportb(0x60) != 0x41)
						{
							ret_code = 7;	// unknown second ID byte, error
						} else
						{
							ret_code = 2;	// first and second ID bytes checkout, this is a MF II keyboard
						};
					};
				};
			};
		};
	};
	return(ret_code);
};

// takes the scancode set, sets the kbd to it
// returns 1 if succes, or 0 upon failer
u_char kbd_set_scancode_set(u_char sc_set)
{
	if(write_kbd_await_ack(0xF0))
	{
		return(write_kbd_await_ack(sc_set));
	};
	return(0);
};

// returns current scancode set, or 0 upon failer
u_char kbd_curr_scancode_set()
{
	write_kbd_await_ack(0xF0);
	write_kbd(0x60, 0);
	return(read_kbd());	// return scancode set
};
