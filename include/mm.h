#ifndef __MM_HEADER
#define __MM_HEADER

struct mm_tracking {
	unsigned long page_usage_bitmap[32768];
	unsigned long superpage_usage_bitmap[32];
	unsigned short superpage_count[1024];
};

struct real_mm_tracking {
	unsigned long page_bitmap[32768];
	unsigned long superpage_bitmap[32];
	unsigned short superpage_count[1024];
	unsigned short superpage_top;
	unsigned page_top;
};

unsigned long total_physical_ram;

// functions in mm.c
unsigned int paging_init();
void enable_paging();
void *real_mem_malloc();
unsigned int real_mem_free(void *PointerToPage);

#endif
