#include <data_types.h>
#include <putchar.h>
#include <tasks.h>

void task_1()
{
	for(;;)
	{
		putc('a');
		putc('b');
		putc('c');
		//asm("int $0x40");
	};
};

void task_2()
{
	for(;;)
	{
		putc('b');
		//asm("int $0x40");
	};
};
