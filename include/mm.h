#ifndef __MM_HEADER
#define __MM_HEADER

// memory management structure (one per address space)
typedef struct
{
	u_long page_bitmap[32767];	// bitmap of which pages are free
	u_long superpage_bitmap[31];	// bitmap of which superpages have free pages
	short superpage_count[1023];	// number of free pages in the superpage
} mm_t;

u_long virt_mem_inti();
void *kmorecore(u_long n);

#endif
