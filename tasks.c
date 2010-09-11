#include <data_types.h>
#include <putchar.h>
#include <keyboard.h>
#include <tasks.h>

void task_1()
{
	for(;;)
	{
		putc('a');
	};
};

void task_2()
{
	for(;;)
	{
		putc('b');
	};
};

void kbd_input_test()
{
	u_char the_char=0;

	for(;;)
	{
		the_char = get_raw_scancode();
		if(the_char != 0)
		{
			k_printf("0x%x\n", the_char);
		};
	};
};
