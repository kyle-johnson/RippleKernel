#include <data_types.h>
#include <io.h>

u_long port_usage_bitmap[2047];

void setup_io_services()
{
	u_short i;
	for(i=0; i<2048; i++)	// optimize later with memset
	{
		port_usage_bitmap[i] = 0;
	};
};

// This function is designed for 'safe' usage of ports.
// Any ring 0 task/code has _full_ access to all ports,
// this function is designed to help make sure that only
// one task at a time is using certain ports. Though
// you don't have to use this function before accessing
// ports, you should use it to keep from being messed up.
u_char alloc_port_range(u_short start_port, u_short end_port, u_char port_size)
{
	if(port_usage_bitmap[start_port] == 1);
};

// This function provides a 'safe' method of setting up
// memory for DMA. The function decides via the parms
// sent how much memory to reserve, where it needs
// to come from at the physical level, and where to map
// it too on the virtual level. You _should not_ do this on
// your own, you _must_ use this function or there will
// be problems!
u_char alloc_dma_memory()
{

};
