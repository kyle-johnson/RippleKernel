// basic putc function
// by K.J.		5.7.2002
#include <k_defines.h>
#include <data_types.h>
#include <string.h>
#include <mutex.h>
#include <putchar.h>

// video memory pointer
char *vidmem2 = (char *) 0xb8000;

unsigned _csr_x=0, _csr_y=0;
u_char putc_mutex=0;

void putc(unsigned char c)
{
	unsigned int vid_mem_spot;

	//lock_mutex_block(&putc_mutex);

	switch(c)
	{
		case '\n':			// new line
			_csr_y++;
			_csr_x = 0;
			break;

		case '\r':			// carrige return
			_csr_y++;
			_csr_x = 0;
			break;

		case '\b':
			_csr_x--;
			break;

		default:			// normal charactor
			vid_mem_spot = ((_csr_y  * 80) + _csr_x);
			vid_mem_spot = vid_mem_spot << 1;	// multiply by 2

			vidmem2[vid_mem_spot] = c;
			vidmem2[vid_mem_spot + 1] = 0x07;	// white on black text
			_csr_x++;
			break;
	};

	if(_csr_x > 79)		// last charactor was at the end of the line
	{
		_csr_y++;	// next line down
		_csr_x = 0;
	};
	if(_csr_y > 24)		// we are at the bottom visable line
	{
		// asm("cli : hlt");
		_memcpy(0xB8000, 0xB8000+(80*2), 24*80*2);	// scroll the video memory up a line
		_memsetw(0xB8000+(24*80*2), 0x0720, 80);	// clear the bottom line
		_csr_y=24;
	};

	update_cursor(_csr_y, _csr_x);

	//unlock_mutex(&putc_mutex);
};
