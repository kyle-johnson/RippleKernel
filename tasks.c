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
		putc('d');
		putc('e');
		putc('f');
		putc('g');
		putc('h');
		putc('i');
		putc('j');
		putc('k');
		putc('l');
		putc('m');
		putc('n');
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
