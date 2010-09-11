#include <data_types.h>
#include <k_printf.h>
#include <string.h>
#include <vbe3.h>

void *FindPMEntryBlock()
{
	u_long *i;
	u_char *s;

	for(s=0xC0000; s<0xC8000; s++)
	{
		if(*s == 'P')
		{
			s++;
			if(*s == 'M')
			{
				s++;
				if(*s == 'I')
				{
					s++;
					if(*s == 'D')
					{
						k_printf("\nVBE 3 PMode access point found.\n");
						return;
					};
				};
			};
		};
	};
	k_printf("\nVBE 3 PMode access point not found.\n");
};
