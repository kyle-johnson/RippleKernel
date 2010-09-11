// multiproccessing stuff(very imcomplete)
#include <data_types.h>
#include <string.h>
#include <k_printf.h>
#include <mp.h>

mp_floating_t *MP_fp_t;

// returns 1 if it is, 0 if it isn't
u_char is_mp_present()
{
	u_long i;
	for(i=0; i<0x100000; i += 16)	// I'm not sure the entire first MB needs to be searched or not
	{
		if(_memcmp(i, "_MP_", 4) == 0) // we got multiprocessing!!!
		{
			MP_fp_t = i;
			if(MP_fp_t->length == 1)
			{
				if(MP_fp_t->specification == 1 || MP_fp_t->specification == 4)
				{
					if(MP_fp_t->MPConfig_p != 0)
					{
						k_printf("YEAH! Multiprocessing sig found @ 0x%x\n", i);
						return 1;
					} else
					{
						k_printf("no MP config table.\n");
						return 0;
					};
				} else
				{
					k_printf("wrong MP specification.\n");
				};
			};
		};
	};
	k_printf("No multiprocessing sig found.\n");
	return 0;
};
