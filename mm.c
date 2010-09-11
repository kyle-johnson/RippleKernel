#include <data_types.h>
#include <k_printf.h>
#include <phys_mm.h>
#include <mm.h>

mm_t *mm_p = (mm_t *) 0x300000;

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

	for(i=0; i<1024; i++)
	{
		page_dir[i] = 2;		// set them to not-present, supervisor level, read/write
	};
	// map the page directory into itself for easy modification
	page_dir[1023] = page_dir;// set to present, supervisor level, read/write, and set this DNS(do not swap, 1st availble bit = 1) 0x203
	page_dir[1023] |= 0x3;

	page_table = pop_phys_mm(0x200000);
	for(i=0; i<1024; i++)	// 0x0 - 0x3FFFFF
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

void *kmorecore(u_long n)
{
	/*
	-----------------------------------------------------------------------------------
	Description:
	Allocates memory in the current virtual address space. 
	Returns a pointer to the newly-allocated memory, -1 on error.
	-----------------------------------------------------------------------------------
	*/
	//u_short data_sel;
	int a, b, c, i, j, k, l, t, f_start, f_size;
	//u_long pgdir_ent, pgtab_ent;

	//asm("pushl %eax");
	//asm("movw %ds, %ax");
	//asm("movw %%ax, %w0" : "=m" (data_sel));
	//asm("popl %eax");

	if(n & 0xFFF)	// if n is not 4KB-aligned, align it
	{
		n &= 0xFFFFF000;
		n += 4096;
	}
	n >>= 12;	// get n in pages
	if(n == 0)
	{
		return (void *)-1;
	}

	// lock address space here

	f_size = 0;	// reset count of contiguous free pages
	for(i=0; i<8; i++)	// search through all 1024 4MB superpages in groups of 32
	{
		if(mm_p->superpage_bitmap[i] != 0xFFFFFFFF)	// if there are free pages in some of the superpages
		{
			a = i << 5;
			for(j=0; j<32; j++)	// find which superpages have free pages
			{
				if(mm_p->superpage_count[a + j] >= ((n - f_size) & 0x3FF))	// if superpage possibly has enough free pages... this will result in a slight *bug*: if a superpage doesn't have enough space, whatever it has at the end should be checked to see if it is contiguous with the next superpage, but this small rare occurrence is hereby sacrificed for performance :-)
				{
					b = (a + j) << 5;
					for(k=0; k<31; k++)	// search through all 1024 pages in groups of 32
					{
						t = mm_p->page_bitmap[b + k];
						if(t == 0x00000000)	// if the 32 pages are all free (32 x 4KB = 128KB)
						{
							if(f_size == 0)	f_start = (b + k) << 5;	// save the start address
							f_size += 32;
						}
						else if(t == 0xFFFFFFFF)	// if the 32 pages are all used
						{
							f_size = 0;
						}
						else	// some are used, some are free; figure out which
						{
							c = (b + k) << 5;
							for(l=0; l<31; l++)
							{
								if(!(t & (1 << l)))	// if page is free
								{
									if(f_size == 0)	f_start = c + l;
									f_size++;
								}
								else
								{
									f_size = 0;
								}
							}
						}
						if(f_size >= n)	// if enough pages have been found
						{
							break;
						}
					}
					if(f_size >= n)
					{
						break;
					}
				}
			}
			if(f_size >= n)
			{
				break;
			}
		}
	}
	if(f_size < n)	// if not enough address space is available
	{
		return (void *)-1;
	}

	// write down the allocation markers for demand-mode allocation
	for(i=f_start; i<f_start+n; i++)
	{
		//if(!(i & 0x3FF))	// if we need to go to the next page table
		//{
		//	pgdir_ent = pgdir[i >> 10];	// read page directory entry
		//	if(pgdir_ent == 0)	// if we need to create the page table
		//	{
		//		pgdir_ent = get_free_pmem_page();	// allocate a page of physical memory
		//		pgdir[i >> 10] = pgdir_ent | 0x03;	// write the page directory entry to create the new page table
		//		_memsetd((void *)&pgtab[i], 0, 1024);	// initialize the new page table
		//		pgdir[i >> 10] |= 0x04;	// allow applications to access it now (the TLB flush at the end will take care of this)
		//	}
		//}
		// pgtab[i] = NP_ALLOC_MAPPED | (data_sel == kdata_sel ? 0 : 0x08) | (i == f_start ? 0 : 0x04);
		mm_p->page_bitmap[i >> 5] |= 1 << (i & 0x1F);	// mark the page as allocated by setting its bit in the bitmap
		mm_p->superpage_count[i >> 10]--;	// subtract one from the superpage's number of free pages
		if(mm_p->superpage_count[i >> 10] == 0)		// if the result is 0, set the superpage's bit in the bitmap
		{
			mm_p->superpage_bitmap[(i >> 10) >> 5] |= 1 << ((i >> 10) & 0x1F);
		}
	}

	// unlock the address space

	return (void *)(f_start << 12);
}

void *morecore(u_long n)
{
	/*
	-----------------------------------------------------------------------------------
	Description:
	Allocates memory in the current virtual address space. 
	Returns a pointer to the newly-allocated memory, -1 on error.
	-----------------------------------------------------------------------------------
	*/
	//u_short data_sel;
	int a, b, c, i, j, k, l, t, f_start, f_size;
	//u_long pgdir_ent, pgtab_ent;

	//asm("pushl %eax");
	//asm("movw %ds, %ax");
	//asm("movw %%ax, %w0" : "=m" (data_sel));
	//asm("popl %eax");

	if(n & 0xFFF)	// if n is not 4KB-aligned, align it
	{
		n &= 0xFFFFF000;
		n += 4096;
	}
	n >>= 12;	// get n in pages
	if(n == 0)
	{
		return (void *)-1;
	}

	// lock address space here

	f_size = 0;	// reset count of contiguous free pages
	for(i=8; i<32; i++)	// search through all 1024 4MB superpages in groups of 32
	{
		if(mm_p->superpage_bitmap[i] != 0xFFFFFFFF)	// if there are free pages in some of the superpages
		{
			a = i << 5;
			for(j=0; j<32; j++)	// find which superpages have free pages
			{
				if(mm_p->superpage_count[a + j] >= ((n - f_size) & 0x3FF))	// if superpage possibly has enough free pages... this will result in a slight *bug*: if a superpage doesn't have enough space, whatever it has at the end should be checked to see if it is contiguous with the next superpage, but this small rare occurrence is hereby sacrificed for performance :-)
				{
					b = (a + j) << 5;
					for(k=0; k<31; k++)	// search through all 1024 pages in groups of 32
					{
						t = mm_p->page_bitmap[b + k];
						if(t == 0x00000000)	// if the 32 pages are all free (32 x 4KB = 128KB)
						{
							if(f_size == 0)	f_start = (b + k) << 5;	// save the start address
							f_size += 32;
						}
						else if(t == 0xFFFFFFFF)	// if the 32 pages are all used
						{
							f_size = 0;
						}
						else	// some are used, some are free; figure out which
						{
							c = (b + k) << 5;
							for(l=0; l<31; l++)
							{
								if(!(t & (1 << l)))	// if page is free
								{
									if(f_size == 0)	f_start = c + l;
									f_size++;
								}
								else
								{
									f_size = 0;
								}
							}
						}
						if(f_size >= n)	// if enough pages have been found
						{
							break;
						}
					}
					if(f_size >= n)
					{
						break;
					}
				}
			}
			if(f_size >= n)
			{
				break;
			}
		}
	}
	if(f_size < n)	// if not enough address space is available
	{
		return (void *)-1;
	}

	// write down the allocation markers for demand-mode allocation
	for(i=f_start; i<f_start+n; i++)
	{
		//if(!(i & 0x3FF))	// if we need to go to the next page table
		//{
		//	pgdir_ent = pgdir[i >> 10];	// read page directory entry
		//	if(pgdir_ent == 0)	// if we need to create the page table
		//	{
		//		pgdir_ent = get_free_pmem_page();	// allocate a page of physical memory
		//		pgdir[i >> 10] = pgdir_ent | 0x03;	// write the page directory entry to create the new page table
		//		_memsetd((void *)&pgtab[i], 0, 1024);	// initialize the new page table
		//		pgdir[i >> 10] |= 0x04;	// allow applications to access it now (the TLB flush at the end will take care of this)
		//	}
		//}
		// pgtab[i] = NP_ALLOC_MAPPED | (data_sel == kdata_sel ? 0 : 0x08) | (i == f_start ? 0 : 0x04);
		mm_p->page_bitmap[i >> 5] |= 1 << (i & 0x1F);	// mark the page as allocated by setting its bit in the bitmap
		mm_p->superpage_count[i >> 10]--;	// subtract one from the superpage's number of free pages
		if(mm_p->superpage_count[i >> 10] == 0)		// if the result is 0, set the superpage's bit in the bitmap
		{
			mm_p->superpage_bitmap[(i >> 10) >> 5] |= 1 << ((i >> 10) & 0x1F);
		}
	}

	// unlock the address space

	return (void *)(f_start << 12);
}


u_long getpagesize()
{
	return 4096;
};
