#include <data_types.h>
#include <k_printf.h>
#include <string.h>
#include <vbe3.h>

void *FindPMEntryBlock()
{
	u_long i;

	for(i=0xF0000; i<0xF8000; i++)
	{
		if(_memcmp(i, "PMID", 4) == 0)
		{
			k_printf("\nVBE 3 PMode access point found.\n");
		}
	};
	k_printf("\nVBE 3 PMode access point not found.\n");
};
