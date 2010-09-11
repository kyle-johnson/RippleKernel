/***********************************************
  VGA only, no SVGA stuff in here.
  This would only be used for maybe
  a splash screen at bootup, and a
  setup utility.

  Most of the code in this file was
  snagged from the the Tauron VGA
  utilities.
************************************************/
#include <data_types.h>
#include <ports.h>
#include <vga.h>

u_char modeC4[62] = {
// MISC reg,  STATUS reg,    SEQ regs
   0x63,      0x00,          0x03,0x01,0x0F,0x00,0x06,
// CRTC regs
0x5F,0x4F,0x50,0x82,0x54,0x80,0x0BF,0x1F,0x00,0x41,0x00,0x00,0x00,0x00,0x00,0x00,
0x9C,0x0E,0x8F,0x28,0x00,0x96,0xB9,0xE3,0xFF,
// GRAPHICS regs
0x00,0x00,0x00,0x00,0x00,0x40,0x05,0x0F,0xFF,
// ATTRIBUTE CONTROLLER regs
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x41,0x00,0x0F,0x00,0x00
};

char Pal[768] = {
 0,  0,  0,  0,  0, 42,  0, 42,  0,  0, 42, 42, 42,  0,  0, 42,  0, 42,
42, 21,  0, 42, 42, 42, 21, 21, 21, 21, 21, 63, 21, 63, 21, 21, 63, 63,
63, 21, 21, 63, 21, 63, 63, 63, 21, 63, 63, 63,  0,  0,  0,  5,  5,  5,
 8,  8,  8, 11, 11, 11, 14, 14, 14, 17, 17, 17, 20, 20, 20, 24, 24, 24,
28, 28, 28, 32, 32, 32, 36, 36, 36, 40, 40, 40, 45, 45, 45, 50, 50, 50,
56, 56, 56, 63, 63, 63,  0,  0, 63, 16,  0, 63, 31,  0, 63, 47,  0, 63,
63,  0, 63, 63,  0, 47, 63,  0, 31, 63,  0, 16, 63,  0,  0, 63, 16,  0,
63, 31,  0, 63, 47,  0, 63, 63,  0, 47, 63,  0, 31, 63,  0, 16, 63,  0,
 0, 63,  0,  0, 63, 16,  0, 63, 31,  0, 63, 47,  0, 63, 63,  0, 47, 63,
 0, 31, 63,  0, 16, 63, 31, 31, 63, 39, 31, 63, 47, 31, 63, 55, 31, 63,
63, 31, 63, 63, 31, 55, 63, 31, 47, 63, 31, 39, 63, 31, 31, 63, 39, 31,
63, 47, 31, 63, 55, 31, 63, 63, 31, 55, 63, 31, 47, 63, 31, 39, 63, 31,
31, 63, 31, 31, 63, 39, 31, 63, 47, 31, 63, 55, 31, 63, 63, 31, 55, 63,
31, 47, 63, 31, 39, 63, 45, 45, 63, 49, 45, 63, 54, 45, 63, 58, 45, 63,
63, 45, 63, 63, 45, 58, 63, 45, 54, 63, 45, 49, 63, 45, 45, 63, 49, 45,
63, 54, 45, 63, 58, 45, 63, 63, 45, 58, 63, 45, 54, 63, 45, 49, 63, 45,
45, 63, 45, 45, 63, 49, 45, 63, 54, 45, 63, 58, 45, 63, 63, 45, 58, 63,
45, 54, 63, 45, 49, 63,  0,  0, 28,  7,  0, 28, 14,  0, 28, 21,  0, 28,
28,  0, 28, 28,  0, 21, 28,  0, 14, 28,  0,  7, 28,  0,  0, 28,  7,  0,
28, 14,  0, 28, 21,  0, 28, 28,  0, 21, 28,  0, 14, 28,  0,  7, 28,  0,
 0, 28,  0,  0, 28,  7,  0, 28, 14,  0, 28, 21,  0, 28, 28,  0, 21, 28,
 0, 14, 28,  0,  7, 28, 14, 14, 28, 17, 14, 28, 21, 14, 28, 24, 14, 28,
28, 14, 28, 28, 14, 24, 28, 14, 21, 28, 14, 17, 28, 14, 14, 28, 17, 14,
28, 21, 14, 28, 24, 14, 28, 28, 14, 24, 28, 14, 21, 28, 14, 17, 28, 14,
14, 28, 14, 14, 28, 17, 14, 28, 21, 14, 28, 24, 14, 28, 28, 14, 24, 28,
14, 21, 28, 14, 17, 28, 20, 20, 28, 22, 20, 28, 24, 20, 28, 26, 20, 28,
28, 20, 28, 28, 20, 26, 28, 20, 24, 28, 20, 22, 28, 20, 20, 28, 22, 20,
28, 24, 20, 28, 26, 20, 28, 28, 20, 26, 28, 20, 24, 28, 20, 22, 28, 20,
20, 28, 20, 20, 28, 22, 20, 28, 24, 20, 28, 26, 20, 28, 28, 20, 26, 28,
20, 24, 28, 20, 22, 28,  0,  0, 16,  4,  0, 16,  8,  0, 16, 12,  0, 16,
16,  0, 16, 16,  0, 12, 16,  0,  8, 16,  0,  4, 16,  0,  0, 16,  4,  0,
16,  8,  0, 16, 12,  0, 16, 16,  0, 12, 16,  0,  8, 16,  0,  4, 16,  0,
 0, 16,  0,  0, 16,  4,  0, 16,  8,  0, 16, 12,  0, 16, 16,  0, 12, 16,
 0,  8, 16,  0,  4, 16,  8,  8, 16, 10,  8, 16, 12,  8, 16, 14,  8, 16,
16,  8, 16, 16,  8, 14, 16,  8, 12, 16,  8, 10, 16,  8,  8, 16, 10,  8,
16, 12,  8, 16, 14,  8, 16, 16,  8, 14, 16,  8, 12, 16,  8, 10, 16,  8,
 8, 16,  8,  8, 16, 10,  8, 16, 12,  8, 16, 14,  8, 16, 16,  8, 14, 16,
 8, 12, 16,  8, 10, 16, 11, 11, 16, 12, 11, 16, 13, 11, 16, 15, 11, 16,
16, 11, 16, 16, 11, 15, 16, 11, 13, 16, 11, 12, 16, 11, 11, 16, 12, 11,
16, 13, 11, 16, 15, 11, 16, 16, 11, 15, 16, 11, 13, 16, 11, 12, 16, 11,
11, 16, 11, 11, 16, 12, 11, 16, 13, 11, 16, 15, 11, 16, 16, 11, 15, 16,
11, 13, 16, 11, 12, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0, 63, 63, 63
};

void SetVidMode(u_char *regs)
{
	u_short i=0, s=0, a=0;

	// send MISC regs
	outportb(MISC_ADDR, regs[i]);
	i++;
	outportb(STATUS_ADDR, regs[i]);
	i++;

	// send SEQ regs
	a = 0;
	while(a!=5)
	{
		outportb(SEQ_ADDR, a);
		outportb((SEQ_ADDR+1), regs[i]);
		i++;
		a++;
	};

	// clear protection bits
	outportw(CRTC_ADDR, 0xE11);

	// send CRTC regs
	a = 0;
	while(a!=25)
	{
		outportb(CRTC_ADDR, a);
		outportb((CRTC_ADDR+1), regs[i]);
		i++;
		a++;
	};

	// send graphics regs
	a = 0;
	while(a!=9)
	{
		outportb(GRACON_ADDR, a);
		outportb((GRACON_ADDR+1), regs[i]);
		i++;
		a++;
	};
	s=inportb(ATTRCON_ADDR);

	// send ATTRCON regs
	a = 0;
	while(a!=21)
	{
		s=inportb(ATTRCON_ADDR);
		outportb(ATTRCON_ADDR, a);
		outportb(ATTRCON_ADDR, regs[i]);
		i++;
		a++;
	};
	outportb(ATTRCON_ADDR, 0x20);
};

void SetModeMODE_X(struct Vmode *Mode)
{
	SetVidMode(&modeC4);

	outportb(MISC_ADDR,0xE3);
	// turn off write protect
	outportw(CRTC_ADDR,0x2C11);
	// vertical total
	outportw(CRTC_ADDR,0xD06);
	// overflow register
	outportw(CRTC_ADDR,0x3E07);
	// vertical retrace start
	outportw(CRTC_ADDR,0xEA10);
	// vertical retrace end AND wr.prot
	outportw(CRTC_ADDR,0xAC11);
	// vertical display enable end
	outportw(CRTC_ADDR,0xDF12);
	// start vertical blanking
	outportw(CRTC_ADDR,0xE715);
	// end vertical blanking
	outportw(CRTC_ADDR,0x616);

	setpalette256();
	Mode->width = 320;
	Mode->height = 240;
	Mode->width_bytes = 19200;
	Mode->colors = 256;
	Mode->attrib = TVU_GRAPHICS | TVU_UNCHAINED;
};

void setpalette256()
{
	u_short j=0, i=0;
	for (i=0; i<768; i+=3)
	{
		setpal(j, Pal[i], Pal[i+1], Pal[i+2]);
		j++;
	};
};

void setpal(u_short color, u_char r, u_char g, u_char b)
{
	outportb(0x3c8, color);
	outportb(0x3c9, r);
	outportb(0x3c9, g);
	outportb(0x3c9, b);
};
