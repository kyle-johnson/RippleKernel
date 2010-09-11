#include <data_types.h>
#include <k_defines.h>
#include <tss.h>
#include <tasks.h>

task_1()
{
	for(;;)
	{
		putc('a');
	};
};

task_2()
{
	for(;;)
	{
		putc('b');
	};
};
