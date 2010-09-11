#ifndef MM_HEADER
#define MM_HEADER

struct
{
	unsigned long page_usage_bitmap[32768];
	unsigned long superpage_usage_bitmap[32];
	unsigned short superpage_count[1024];
} mm_tracking;

unsigned long total_physical_ram;

// functions in mm.c
unsigned int paging_init();
void enable_paging();

#endif
