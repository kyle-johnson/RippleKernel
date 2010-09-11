#include <data_types.h>
#include <k_printf.h>
#include <phys_mm.h>
#include <mm.h>

// Virtual memory intilization function. Assumes that
// physical memory management has allready been
// started and that there is a stack of free physical
// pages at 0x200000.
u_long virt_mem_inti()
{
	u_long *page_dir;
	u_long *page_table;
	u_long i, address=0;

	page_dir = pop_phys_mm(0x200000);

	for(i=0; i<1023; i++)
	{
		page_dir[i] = 2;		// set them to not-present, supervisor level, read/write
	};
	// map the page directory into itself for easy modification
	page_dir[1023] = page_dir;// set to present, supervisor level, read/write, and set this DNS(do not swap, 1st availble bit = 1) 0x203
	page_dir[1023] |= 0x3;

	page_table = pop_phys_mm(0x200000);
	for(i=0; i<1023; i++)	// 0x0 - 0x3FFFFF
	{
		page_table[i] = (u_long)address | 3;
		address += 0x1000;
	};
	page_table[1] = page_dir; // we can now have direct access to the page_dir @ 0x1000
	page_table[1] |= 0x3; // set to present, supervisor level, read/write, and set this DNS(do not swap, 1st availble bit = 1) 0x203

	page_dir[0] = page_table;	// map the page table into the page directory
	page_dir[0] |= 3;

	return page_dir;
};

// allocates 1 page of memory and returns that page's address(kernel use only)
void *kpage_alloc()
{
	u_long i;

	for(i=0; i<32767; i++)
	{
		if(abc.page_bitmap[i] != 0xFFFFFFFF)
		{
			if(abc.page_bitmap[i] == 0x0)
			{
				abc.page_bitmap[i] = 1;
				return(i << 12);
			};
			
		};
	};
};
