#ifndef __VGA_HEADER
#define __VGA_HEADER

/////////////////////////////////////////////////////////////////////////////////////////

#define SEQ_ADDR		0x03C4
#define GRACON_ADDR		0x03CE
#define CRTC_ADDR		0x03D4

// VGA register port addresses
#define ATTRCON_ADDR		0x03C0
#define MISC_ADDR		0x03C2
#define VGAENABLE_ADDR		0x03C3
#define SEQ_ADDR		0x03C4
#define GRACON_ADDR		0x03CE
#define CRTC_ADDR		0x03D4
#define STATUS_ADDR		0x03DA

// Videomode attributes
#define TVU_TEXT			0x0001
#define TVU_GRAPHICS		0x0002
#define TVU_MONOCHROME		0x0004
#define TVU_PLANAR		0x0008
#define TVU_UNCHAINED		0x0010

// Video modes
#define MODE_X			0x15
#define CHAIN4			0x14

// Videomode Info Structure
struct Vmode {
	u_short mode,	// Videomode Number
	width,		// Width in pixels
	height,		// Height in pixels
	width_bytes,	// Number of bytes per screen
	colors,		// Number of colors
	attrib;		// Videomode attributes
};

/////////////////////////////////////////////////////////////////////////////////////////

void SetVidMode(u_char *regs);
void SetModeMODE_X(struct Vmode *Mode);
void setpalette256();
void setpal(u_short color, u_char r, u_char g, u_char b);

#endif
