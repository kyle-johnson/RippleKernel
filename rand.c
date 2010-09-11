// creates random numbers
#include <data_types.h>
#include <rand.h>

static u_short __rand_seed;

u_long rand() // this doesn't work too well it seems
{
	if(__rand_seed == 0)
	{
		__rand_seed = 1;
	};
	if((((__rand_seed << 3) ^ __rand_seed) & 0x80000000uL) != 0)
	{
		__rand_seed = (__rand_seed << 1) | 1;
	} else {
		__rand_seed <<= 1;
	};

	return(__rand_seed - 1);
};

void rseed(u_short seed)
{
	__rand_seed = seed;
};
