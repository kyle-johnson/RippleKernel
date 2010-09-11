#include <data_types.h>
#include <mutex.h>
#include <mm.h>

// stucture for tracking REAL PHYSICAL memory
struct real_mm_tracking r_m_t;

// memory management mutex
volatile u_char mm_mutex = 0;

// paging startup/setup
unsigned int paging_init()
{
	unsigned long 	*mem_block = (unsigned long *) 0x9C000;
	unsigned long 	*pagetab,	// page table address
			*pagetab2,
			*pagedir;	// page directory address
	unsigned long addr, pagetab_spot, pagedir_spot;
	unsigned long i, s;

	lock_mutex_block(&mm_mutex);

	// zero the page directory
	for(i=0; i<1024; i++)
	{
		mem_block[i] = 0x00000000;
	};

	// now, setup pagedir to start at the page directory
	pagedir = mem_block;

	// next, setup pagetab to start at the page table
	pagetab = pagedir + (1024*4);
	pagetab2 = pagedir + (2*(1024*4));

	// point the 1st PDE to the 1st PT
	pagetab_spot = (unsigned long)pagedir + (1*(1024*4));
	pagedir[0] = pagetab_spot | 3; // supervisor level, read/write, present(011 in binary)

	// finish filling in PDEs for the first 1GB of address space
	for(i=1;i<249;i++)
	{
		pagetab_spot = (unsigned long)pagedir + ((i+1)*(1024*4));
		pagedir[i] = pagetab_spot | 2; // supervisor level, read/write, not present(010 in binary)
	};
	// this is for the user address space
	for(i=250;i<999;i++)
	{
		pagetab_spot = (unsigned long)pagedir + ((i+1)*(1024*4));
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
	write_cr3(pagedir_spot);

	r_m_t.superpage_top = 4; // 4 x 4 = 16mb
	r_m_t.page_top = (r_m_t.superpage_top * 256);

	// clear the bitmap for 4kb pages
	u_short mem_var;
	for(mem_var=1; mem_var<(r_m_t.page_top << 5); mem_var++)
	{
		r_m_t.page_bitmap[mem_var-1] = 0x00000000;
	};
	// clear the superpage bitmap
	r_m_t.superpage_bitmap[0] = 0x00000000;


	// now we take memory(4mb) for the kernel, page tables, etc
	r_m_t.superpage_bitmap[0] = 1; // bottom 4mb taken
	// reserve all the 4k pages for that bottom 4mb
	for(mem_var=0; mem_var<31; mem_var++)
	{
		r_m_t.page_bitmap[mem_var] = 0xFFFFFFFF;
	};
	// we have all 4k pages in use for the bottom 4mb
	r_m_t.superpage_count[0] = 0;

	unlock_mutex(&mm_mutex);

	return(1);
};

void enable_paging()
{
	// set bit 31 in CR0 to 1 to enable paging
	write_cr0(read_cr0() | 0x80000000L);
};

/***************************************************************
*  This function deals only with PHYSICAL memory
*  and physical memory only
*
*  This function can allocat NO LESS than 4kb of memory
*  and NO MORE than 4kb of memory at a time
*
*  The overall design of this function comes from Frank
*  Millea's Cottontail OS
****************************************************************/
void *real_mem_malloc()
{
	unsigned int i, a, b, c, d, e, f, g, h;
	unsigned int PointerToPage, found=0;

	lock_mutex_block(&mm_mutex);

	// this is necessary because we have 32 4mb "spaces" for each variable in the superpage_bitmap array
	if(r_m_t.superpage_top<=32)
	{
		if(r_m_t.superpage_bitmap[0] != 0xFFFFFFFF) // there's free mem somewhere here
		{
			a = 0 << 5; // will all ways be 0 for this case
			for(b=0; b<(r_m_t.superpage_top-1); b++)
			{
				if(r_m_t.superpage_count[a+b] >= 1) // hey! at least 1 free 4k page exists in here :)
				{
					c = (a+b) << 5;
					for(d=0; d<32; d++)
					{
						e = r_m_t.page_bitmap[c+d];
						if(e == 0xFFFFFFFF)
						{
							break; // no free pages in this group... go onto the next
						}
						else if(e == 0x00000000) // this group is all free
						{
							PointerToPage = (d + c) << 5; // save the address
							found = 1;
						}
						else // some pages in this group are in use, some aren't
						{
							f = (c+d) << 5;
							for(g=0; g<32; g++)
							{
								if(!(e & (1 << g))) // see if this page is free
								{
									PointerToPage = (f+g);
									found = 1;
								};
							};
						};
					};
				};
			};
		};
		if(found = 0) // no RAM was available
		{
			return (void *)-1;
		};

		// now we need to do some "book keeping"
		// we can change any variable now except "PointerToPage"
		i=PointerToPage;
		r_m_t.page_bitmap[i >> 5] |= 1 << (i & 0x1F);
		r_m_t.superpage_count[i >> 10]--;
		if(r_m_t.superpage_count[i >> 10] == 0)
		{
			r_m_t.superpage_bitmap[(i >> 10) >> 5] |= 1 << ((i >> 10) & 0x1F);
		};
	};

	if(r_m_t.superpage_top>32)
	{
		for(h=0; h<(r_m_t.superpage_top-1); h++)
		{
			if(r_m_t.superpage_bitmap[h] != 0xFFFFFFFF) // there's free mem somewhere here
			{
				a = h << 5; // will all ways be 0 for this case
				for(b=0; b<(r_m_t.superpage_top-1); b++)
				{
					if(r_m_t.superpage_count[a+b] >= 1) // hey! at least 1 free 4k page exists in here :)
					{
						c = (a+b) << 5;
						for(d=0; d<32; d++)
						{
							e = r_m_t.page_bitmap[c+d];
							if(e == 0xFFFFFFFF)
							{
								break; // no free pages in this group... go onto the next
							}
							else if(e == 0x00000000) // this group is all free
							{
								PointerToPage = (d + c) << 5; // save the address
								found = 1;
							}
							else // some pages in this group are in use, some aren't
							{
								f = (c+d) << 5;
								for(g=0; g<32; g++)
								{
									if(!(e & (1 << g))) // see if this page is free
									{
										PointerToPage = (f+g);
										found = 1;
									};
								};
							};
						};
					};
				};
			};
			if(found = 0) // no RAM was available
			{
				return (void *)-1;
			};
			
			// now we need to do some "book keeping"
			// we can change any variable now except "PointerToPage"
			i=PointerToPage;
			r_m_t.page_bitmap[i >> 5] |= 1 << (i & 0x1F);
			r_m_t.superpage_count[i >> 10]--;
			if(r_m_t.superpage_count[i >> 10] == 0)
			{
				r_m_t.superpage_bitmap[(i >> 10) >> 5] |= 1 << ((i >> 10) & 0x1F);
			};
		};
	};

	unlock_mutex(&mm_mutex);

	// return a pointer to the page
	return (void *)(PointerToPage << 12);
};

/*************************************************
*  This function simply edits the bitmaps for
*  one 4kb page of memory that is reserved
*  via real_mem_malloc()
*
*  Will return and error code if this fails, or
*  a 0 if it succedes
*
* This function is based on the dealloc
*  function from Frank Millea's Cottontail OS
**************************************************/
unsigned int real_mem_free(void *PointerToPage)
{
	unsigned int i = (unsigned int)PointerToPage >> 12;

	lock_mutex_block(&mm_mutex);

	r_m_t.page_bitmap[i >> 5] &= ~(1 << (i & 0x1F));
	r_m_t.superpage_bitmap[(i >> 10) >> 5] &= ~(1 << (i & 0x1F));
	r_m_t.superpage_count[i >> 10]++;

	unlock_mutex(&mm_mutex);

	return 0;
};

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
