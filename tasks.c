#include <data_types.h>
#include <k_defines.h>
#include <tss.h>
#include <tasks.h>

task_1()
{
	struct TSS *our_tsses = (struct TSS *) 0x200000;
	putc('a');
	putc('a');
	our_tsses[1].link = 6;
	putc('a');
};

task_2()
{
	for(;;)
	{
		putc('b');
	};
};
