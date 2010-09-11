#ifndef __VGA_UTILS_HEADER
#define __VGA_UTILS_HEADER

void UnchainedClear(char color, u_short width_bytes);
void _plot_pixel(u_short x, u_short y, u_short screen_width, u_char color);

#endif
