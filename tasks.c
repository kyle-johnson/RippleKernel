#include <tasks.h>

task_1()
{
	putc('a');
	asm("int $0x31");
};

task_2()
{
	while(1)
	{
		putc('b');
	};
};
