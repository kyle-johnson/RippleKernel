#include <mm.h>
#include <data_types.h>

// stucture for tracking REAL PHYSICAL memory
struct real_mm_tracking r_m_t;

// paging startup/setup
unsigned int paging_init()
{
	unsigned long 	*mem_block = (unsigned long *) 0x9C000;
	unsigned long 	*pagetab,	// page table address
			*pagetab2,
			*pagedir;	// page directory address
	unsigned long addr, pagetab_spot, pagedir_spot;
	unsigned long i, s;

	// zero the page directory
	for(i=0; i<1024; i++)
	{
		mem_block[i] = 0x00000000;
	};

	// now, setup pagedir to start at the page directory
	pagedir = mem_block;

	// next, setup pagetab to start at the page table
	pagetab = pagedir + 1024;
	pagetab2 = pagedir + (2*1024);

	// point the 1st PDE to the 1st PT
	pagetab_spot = (unsigned long)pagedir + (1*1024);
	pagedir[0] = pagetab_spot | 3; // supervisor level, read/write, present(011 in binary)

	// finish filling in PDEs for the first 1GB of address space
	for(i=1;i<249;i++)
	{
		pagetab_spot = (unsigned long)pagedir + ((i+1)*1024);
		pagedir[i] = pagetab_spot | 2; // supervisor level, read/write, not present(010 in binary)
	};
	// this is for the user address space
	for(i=250;i<999;i++)
	{
		pagetab_spot = (unsigned long)pagedir + ((i+1)*1024);
		pagedir[i] = pagetab_spot | 6; // user level, read/write, not present(110 in binary)
	};

	// time to fill in the 1st PT
	addr = 0;
	for(i=0; i<1024; i++)
	{
		pagetab[i] = addr | 3;	// supervisor level, read/write, present(011 in binary)
		addr += 4096;		// next page(each page is 4kb)
	};

	// fill in the rest of the PTs for the bottom 1gb
//	addr = 1024;
//	for(i=0; i<254976; i++)
//	{
//		pagetab2[i] = addr | 2;	// supervisor level, read/write, not present(010 in binary)
//		addr += 4096;		// next page(each page is 4kb)
//	};

	/****************************************
	Okay, the page directory, page
	tables, and pages are setup.
	****************************************/
	pagedir_spot = (unsigned long)pagedir;
	// okay, set up CR3 with the address of the PD
	write_cr3((read_cr3() & 0xFFFL) | (pagedir_spot));

	r_m_t.superpage_top = 4;
	r_m_t.page_top = (r_m_t.superpage_top * 256);

	u_short mem_blah;
	for(mem_blah=0; mem_blah<=31; mem_blah++)
	{
		r_m_t.page_bitmap[mem_blah] = 0xFFFFFFFF;
	};

	return(1);
};

void enable_paging()
{
	// set bit 31 in CR0 to 1 to enable paging
	write_cr0(read_cr0() | 0x80000000L);
};

//   This function deals only with PHYSICAL memory
//  and physical memory only
void *real_mem_malloc()
{
	unsigned int i;

	for(i=0; i<(r_m_t.superpage_top-1); i++)
	{
		if(r_m_t.superpage_bitmap[i] != 0xFFFFFFFF) // there's some free mem here
		{
			
		};
	};
};

/************************************************
    The overall design of the mm tracking
  came from Frank  Millea's Cottontail OS
  file "mem.c"
    This function uses 2 bitmaps... a 4mb
  "superpage" bitmap, and then one for
  the actual 4k pages.
************************************************/
//void *k_malloc()
//{
//	unsigned int i, a, b, c, d, t;
//
//	for(i=0; i<32; i++)
//	{
//		if(mm_tracking.superpage_usage_bitmap[i] != 0xFFFFFFFF) // a superpage in this "group" has a free 4k page(s)
//		{
//			a=i<<5; // shift i by 5 and then store the value in a
//			for(b=0; b<32; b++)
//			{
//				if(mm_tracking.superpage_count[a+b] >= i) // see if this superpage enough free 4k page(s)
//				{
//				c = (a+b) << 5;	// offset to start of 4k pages
//					for(d=0; d<32; d++)	// go through all 1024 4k pages in groups of 32
//				{
//						t = mm_tracking.page_usage_bitmap[c+d];
//						if(t == 0x00000000)	// all 32 4k pages are free
//						{
//							;
//						}
//						else if(t == 0xFFFFFFFF)	// all 32 4k pages are taken
//						{
//							;
//						}
//						else	// some of the 4k pages are used an some aren't
//						{
//							;
//						};
//					};
//				};
//			};
//		};
//	};
//};

// taken from Frank  Millea's Cottontail OS file, "mem.c"
//u_long memprobe()
//{
//	#define MEMPROBE_MAGIC 0x5A5AA5A5
//	#define PADDR_LIMIT 0xBFFFFFFF	// physical (conventional) memory limit is at 3GB
	/*
	-----------------------------------------------------------------------------------
	Probes memory at 4KB intervals and returns system memory size.
	-----------------------------------------------------------------------------------
	*/
//	u_long *m = (u_long *)0;
//	u_long i = 0x40000, j, temp;

//	temp = m[i];	// save copy of what we will be modifying
//	m[i] = MEMPROBE_MAGIC;
//	while(m[i] == MEMPROBE_MAGIC && (i << 2) < PADDR_LIMIT + 1)
//	{
//		m[i] = temp;	// restore value in memory
//		i += 0x400;	// go to the next page
//		temp = m[i];	// save copy of what we will be modifying
//		m[i] = MEMPROBE_MAGIC;
//		for(j=0; j<100; j++){};	// short delay loop (some systems will lock or report incorrect sizes if we don't delay here)
//	};
//	return i << 2;
//};
