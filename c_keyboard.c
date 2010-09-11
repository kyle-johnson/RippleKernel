#define __ERRNO 0x80000000

#define E_NB_LOCK_FAILED (__ERRNO+2)
#define E_WOULD_BLOCK (__ERRNO+3)
#define E_TIMED_OUT (__ERRNO+4)
#define E_OVERFLOW (__ERRNO+7)
#define E_INVALID_ID (__ERRNO+8)
#define E_OBJECT_FULL (__ERRNO+9)
#define E_OBJECT_EMPTY (__ERRNO+10)
#define E_NO_OBJECT (__ERRNO+11)
#define E_NOT_ALLOWED (__ERRNO+12)
#define E_NONBLOCKING (__ERRNO+13)
#define E_BAD_OBJECT (__ERRNO+14)
#define E_MALLOC (__ERRNO+15)

#define E_MEDIA_EJECTED (__ERRNO+500)

#define E_KEYBOARD_UNPLUGGED (__ERRNO+1000)
#define E_KEYBOARD_NOT_MF2 (__ERRNO+1001)
#define E_KEYBOARD_ERROR (__ERRNO+1002)
#define E_PS2M_ERROR (__ERRNO+1003)
#define E_PS2M_UNPLUGGED (__ERRNO+1004)

#define E_WRONG_FS_TYPE (__ERRNO+1500)



#include <data_types.h>
#include <keyboard.h>
#include <mutex.h>
#include "c_keyboard.h"
#include "c_scanset2.c"

int keyboard_inbuf[32] = {}, keyboard_inbuf_len = 0;
int keyboard_datbuf[32] = {}, keyboard_datbuf_len = 0;

char keys_pressed[256] = {};
char keyboard_led_map = 0;
char ps2m_spawned = 0;
u_char keyboard_mutex = 0;

void keyboard_main()
{
	/*
	-----------------------------------------------------------------------------------------------------
	Description:
	Main function of keyboard driver.
	-----------------------------------------------------------------------------------------------------
	*/
	u_long s = 0;
	int key, ascii_key;
	int led_keys_pressed[3];

//	watch_irq(1);	// watch IRQ1

	for(;;)
	{
		// if keyboard I/O device can be found, unregister it
		if(keyboard_init() == 0)	// if keyboard was initialized successfully
		{
			k_printf("\nKeyboard intilized!\n");
			for(;;)
			{
			//	k_printf("^");
				keyboard_inbuf[keyboard_inbuf_len] = keyboard_byte_in();	// read the data in
			//	k_printf("*");
				keyboard_inbuf_len++;

				// save pressed state of LEDs
				led_keys_pressed[0] = keys_pressed[KEY_SCRLOCK];
				led_keys_pressed[1] = keys_pressed[KEY_NUMLOCK];
				led_keys_pressed[2] = keys_pressed[KEY_CAPSLOCK];

				key = keyboard_decode_inbuf();	// attempt to decode it

				if((keys_pressed[KEY_LALT] || keys_pressed[KEY_RALT]) && (keys_pressed[KEY_LCTRL] || keys_pressed[KEY_RCTRL]) && (keys_pressed[KEY_DEL] || keys_pressed[KEY_N_DOT]))
				{
					k_printf("\n\nreboot\n\n");;
				}

				if(key != 0)	// if a key value was obtained
				{
					if(key == KEY_SCRLOCK || key == KEY_NUMLOCK || key == KEY_CAPSLOCK)
					{
						if(!led_keys_pressed[0] && keys_pressed[KEY_SCRLOCK])
						{
							keyboard_led_map ^= 0x01;
							if(set_keyboard_leds(keyboard_led_map) < 0)	break;	// set LEDs
						}
						else if(!led_keys_pressed[1] && keys_pressed[KEY_NUMLOCK])
						{
							keyboard_led_map ^= 0x02;
							if(set_keyboard_leds(keyboard_led_map) < 0)	break;
						}
						else if(!led_keys_pressed[2] && keys_pressed[KEY_CAPSLOCK])
						{
							keyboard_led_map ^= 0x04;
							if(set_keyboard_leds(keyboard_led_map) < 0)	break;
						}
					}
					else
					{
						ascii_key = keyboard_get_ascii(key);	// get the ASCII value of the key
//						keyboard_datbuf[keyboard_datbuf_len] = key;	// save the key in the data buffer
//						keyboard_datbuf_len++;
						k_printf("%c", ascii_key);
					}
				}
			}
		}
		else
		{
			while(s<0xFFFFFF)
			{
				s++;
			}			
			//sleep(2 * SI_S);	// wait 2 seconds before attempting to detect and init again
		}
	}
}

int keyboard_init()
{
	/*
	-----------------------------------------------------------------------------------------------------
	Description:
	Detects and initializes the keyboard.
	-----------------------------------------------------------------------------------------------------
	*/
	int r;

//	r = keyboard_reset();
//	if(r != 0xAA)	return r;	// if basic assurance test failed
//	r = identify_keyboard();	// identify keyboard as MF II
//	if(r != 0)	return r;
//	r = set_keyboard_leds(0x00);	// all LEDs off
//	if(r != 0)	return r;
//	 r = set_keyboard_scancode_set(2);	// select scancode set 2 (works with my laptop, as well as numerous other computers - also is the default)
//	r = kbd_set_scancode_set(2);
//	if(r != 2)	return r;

	return 0;
}

int identify_keyboard()
{
	/*
	---------------------------------------------------------------------------------------
	Description:
	Identifies the keyboard as an MF II (or not).
	---------------------------------------------------------------------------------------
	*/
	int j, r[3];

        lock_mutex_block(&keyboard_mutex);

	r[0] = keyboard_command_byte(0xF2);	// send the command
	if(r[0] != 0)
	{
		keyboard_mutex = 0;
		return r[0];
	}

	r[1] = keyboard_byte_in();
	r[2] = keyboard_byte_in();
	keyboard_mutex = 0;

	if(r[1] != 0xAB || (r[2] != 0x41/*MF2 translation mode*/ && r[2] != 0x83/*MF2 pass-through mode*/))	// if not an MF II keyboard
	{
		return E_KEYBOARD_NOT_MF2;
	}
	else
	{
		return 0;
	}
}

int set_keyboard_leds(u_char leds)
{
	/*
	---------------------------------------------------------------------------------------
	Description:
	Turns keyboard LEDs on or off (NumLock, CapsLock, ScrollLock)
	---------------------------------------------------------------------------------------
	*/
	int i, r;

        lock_mutex_block(&keyboard_mutex);
	for(i=0; i<2; i++)
	{
		r = keyboard_command_byte((i == 0 ? 0xED : leds));	// send the command byte
		if(r != 0)
		{
			keyboard_mutex = 0;
			return r;
		}
	}
	keyboard_mutex = 0;
	return 0;
}

int set_keyboard_scancode_set(int scanset)
{
	/*
	---------------------------------------------------------------------------------------
	Description:
	Sets the keyboard's scancode set.
	---------------------------------------------------------------------------------------
	*/
	int i, r;

        lock_mutex_block(&keyboard_mutex);

	for(i=0; i<2; i++)
	{
		r = keyboard_command_byte((i == 0 ? 0xF0 : scanset));	// send the command bytes
		if(r != 0)
		{
			keyboard_mutex = 0;
			return r;
		}
	}

	keyboard_mutex = 0;
	return 0;
}

int keyboard_command_byte(u_char byte)
{
	/*
	---------------------------------------------------------------------------------------
	Description:
	Sends a command byte to the keyboard and reads the response (always an ACK).
	---------------------------------------------------------------------------------------
	*/
	int i, r;

	for(i=0; i<500; i++)	// retry up to 10 times
	{
		keyboard_byte_out(0x60, byte);	// send the command byte

		do	// read the response, ignoring garbage data (we want an ACK)
		{
			r = keyboard_byte_in();
		}
		while(r != 0x00 && r != 0xFA && r != 0xFE && r != 0xFF);

		if(r == 0xFA)
		{
			return 0;
		}

		if(r == 0x00 || r == 0xFF)
		{
			return E_KEYBOARD_ERROR;
		}

		// sleep(5 * SI_MS);
	}
	return E_KEYBOARD_UNPLUGGED;
}

void keyboard_byte_out(unsigned short port, unsigned char byte)
{
	/*
	---------------------------------------------------------------------------------------
	Description:
	Sends a byte to the keyboard (either to port 0x60 or 0x64).
	---------------------------------------------------------------------------------------
	*/
	while(inportb(0x64) & 0x02);	// wait for keyboard's output buffer to be empty
	outportb(port, byte);
}

int keyboard_byte_in()
{
	/*
	---------------------------------------------------------------------------------------
	Description:
	Reads a byte from the keyboard's data port (0x60), led by either IRQ1 or IRQ12.
	---------------------------------------------------------------------------------------
	*/
//	wait_irq(-1);
//	while(!(inportb(0x64) & 0x01));	// wait for input buffer to be full
//	return inportb(0x60);
	return(get_raw_scancode());
}

int keyboard_decode_inbuf()
{
	/*
	---------------------------------------------------------------------------------------
	Description:
	Attempts to decode the contents of the keyboard's scancode input buffer, producing
	a nonzero generic key value on success and updating that key's pressed/not pressed flag.
	This key value is returned only if it is a make code, otherwise zero is returned.
	---------------------------------------------------------------------------------------
	*/
	int key, adjust, i;
	char make_code = 0, break_code = 0;

	if(keyboard_inbuf_len >= 1 && keyboard_inbuf[0] != 0xE0 && keyboard_inbuf[0] != 0xE1 && keyboard_inbuf[0] != 0xF0)	// normal make
	{
		key = set2_make_normal[keyboard_inbuf[0]];
		make_code = 1;
		adjust = 1;
	}
	else if(keyboard_inbuf_len >= 2 && keyboard_inbuf[0] == 0xE0 && keyboard_inbuf[1] != 0x12 && keyboard_inbuf[1] != 0xF0)	// short extended make code
	{
		key = set2_make_extended[keyboard_inbuf[1]];
		make_code = 1;
		adjust = 2;
	}
	else if(keyboard_inbuf_len >= 4 && keyboard_inbuf[0] == 0xE0 && keyboard_inbuf[1] == 0x12 && keyboard_inbuf[2] == 0xE0)	// long extended make code
	{
		key = set2_make_extended[keyboard_inbuf[3]];
		make_code = 1;
		adjust = 4;
	}
	else if(keyboard_inbuf_len >= 8 && keyboard_inbuf[0] == 0xE1 && keyboard_inbuf[1] == 0x14 && keyboard_inbuf[2] == 0x77 && keyboard_inbuf[3] == 0xE1 && keyboard_inbuf[4] == 0xF0 && keyboard_inbuf[5] == 0x14 && keyboard_inbuf[6] == 0xF0 && keyboard_inbuf[7] == 0x77)	// make code for Pause/Break
	{
		key = KEY_PAUSE;
		make_code = 1;
		adjust = 8;
	}
	else if(keyboard_inbuf_len >= 2 && keyboard_inbuf[0] == 0xF0)	// normal break code
	{
		key = set2_make_normal[keyboard_inbuf[1]];
		break_code = 1;
		adjust = 2;
	}
	else if(keyboard_inbuf_len >= 3 && keyboard_inbuf[0] == 0xE0 && keyboard_inbuf[1] == 0xF0 && keyboard_inbuf[2] != 0x12)	// extended break code (long or short)
	{
		key = set2_make_extended[keyboard_inbuf[2]];
		break_code = 1;
		adjust = 3;
	}
	else if(keyboard_inbuf_len >= 3 && keyboard_inbuf[0] == 0xE0 && keyboard_inbuf[1] == 0xF0 && keyboard_inbuf[2] == 0x12)	// crap we don't use from a long extended break code
	{
		adjust = 3;
	}
	else	// something unintelligible so far... (for byte sequences > 1)
	{
		key = 0;
		if(keyboard_inbuf_len >= 8)
		{
			k_printf("AIEE! DUMP(%d):", keyboard_inbuf_len);
			for(i=0; i<keyboard_inbuf_len; i++)
			{
				k_printf("%x-", keyboard_inbuf[i]);
			}
			keyboard_inbuf_len = 0;
		}
		adjust = 0;
	}

	keyboard_inbuf_len -= adjust;
	_memcpy(&keyboard_inbuf[0], &keyboard_inbuf[adjust], keyboard_inbuf_len);
	if(make_code)
	{
		keys_pressed[key] = 1;
		return key;
	}
	else if(break_code)
	{
		keys_pressed[key] = 0;
	}
	return 0;
}

int keyboard_get_ascii(int key)
{
	/*
	---------------------------------------------------------------------------------------
	Description:
	This function will take the generic key value and convert it into an ASCII or extended
	ASCII value and return it. If this key cannot be converted, a value of zero is returned.
	---------------------------------------------------------------------------------------
	*/
	int a = 0, b = 0;

	if(keyboard_led_map & 0x02)
	{
		if(keys_pressed[KEY_LSHIFT] || keys_pressed[KEY_RSHIFT])
		{
			a = set2_ascii_numlock_shift[key];
		}
		else
		{
			a = set2_ascii_numlock[key];
		}
	}
	else if(keys_pressed[KEY_LSHIFT] || keys_pressed[KEY_RSHIFT])
	{
		a = set2_ascii_shift[key];
	}
	else
	{
		a = set2_ascii_no_numlock_shift[key];
	}

	b = a;

	if(keys_pressed[KEY_LCTRL] || keys_pressed[KEY_RCTRL])	// if ctrl is pressed
	{
		if(a >= 'a' && a <= 'z')
		{
			b = CTRL_A + a - 'a';
		}
		else if(a >= 'A' && a <= 'Z')
		{
			b = CTRL_A + a - 'A';
		}
		else if(key == KEY_2)
		{
			b = CTRL_AT;
		}
		else if(a == '[')
		{
			b = CTRL_LBRACK;
		}
		else if(a == '\\' || a == '|')
		{
			b = CTRL_BACKSLASH;
		}
		else if(a == ']')
		{
			b = CTRL_RBRACK;
		}
		else if(key == KEY_6)
		{
			b = CTRL_CARET;
		}
		else if(key == KEY_MINUS)
		{
			b = CTRL_USCORE;
		}
		else if(key >= KEY_F1 && key <= KEY_F12)
		{
			b = set2_ascii_ctrl_f[key - KEY_F1];
		}
		else if(a == EXT_LEFT)
		{
			b = EXT_CTRL_LEFT;
		}
		else if(a == EXT_RIGHT)
		{
			b = EXT_CTRL_RIGHT;
		}
		else if(a == EXT_END)
		{
			b = EXT_CTRL_END;
		}
		else if(a == EXT_HOME)
		{
			b = EXT_CTRL_HOME;
		}
		else if(a == EXT_PGDN)
		{
			b = EXT_CTRL_PGDN;
		}
		else if(a == EXT_PGUP)
		{
			b = EXT_CTRL_PGUP;
		}
		else if(a == 8)
		{
			b = EXT_CTRL_TAB;
		}
	}
	else if(keys_pressed[KEY_LALT] || keys_pressed[KEY_RALT])	// if ALT is pressed
	{
		if(a >= 'a' && a <= 'z')
		{
			b = set2_ascii_alt_alpha[a - 'a'];
		}
		else if(a >= 'A' && a <= 'Z')
		{
			b = set2_ascii_alt_alpha[a - 'A'];
		}
		else if(key >= KEY_1 && key <= KEY_0)
		{
			b = set2_ascii_alt_num[key - KEY_1];
		}
		else if(key >= KEY_F1 && key <= KEY_F12)
		{
			b = set2_ascii_alt_f[key - KEY_F1];
		}
	}
	if(keyboard_led_map & 0x04)	// if CAPS is on
	{
		if(b >= 'a' && b <= 'z')
		{
			b = b + 'A' - 'a';
		}
		else if(b >= 'A' && b <= 'Z')
		{
			b = b + 'a' - 'A';
		}
	}
	return b;
}
