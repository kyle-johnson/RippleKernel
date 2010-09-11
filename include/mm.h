#ifndef __MM_HEADER
#define __MM_HEADER

typedef struct {
	u_long page_bitmap[32767];
} page_tracking_t;

u_long virt_mem_inti();

#endif
