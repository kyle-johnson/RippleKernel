#include <data_types.h>
#include <mutex.h>
#include <k_printf.h>
#include <tasks.h>
#include <threads.h>
#include <processes.h>
#include "c_keyboard.h"
#include <scheduler.h>

extern __total_num_threads;

u_char scheduler_mutex=0;
u_char i=0;

thread_struct My_Threads[1];

void make_threads()
{
	create_thread(&My_Threads[0], 0, (u_long)&cool_down_thread, 0x08, 0x10, 0x08, 0);
//	create_thread(&My_Threads[1], 0, (u_long)&keyboard_main, 0x08, 0x10, 0x08, 0);
	create_thread(&My_Threads[1], 0, (u_long)&kbd_input_test, 0x08, 0x10, 0x08, 0);
//	create_thread(&My_Threads[2], 0, (u_long)&task_2, 0x08, 0x10, 0x08, 0);
//	create_thread(&My_Threads[3], 0, (u_long)&task_2, 0x08, 0x10, 0x08);
//	create_thread(&My_Threads[4], 0, (u_long)&task_1, 0x08, 0x10, 0x08);
//	create_thread(&My_Threads[1], 0, (u_long), 0x08, 0x10, 0x08);
};

void *the_scheduler(u_long esp)
{
//	putc('z');
//	putc('z');
//	putc('z');

	i++;
	if(i <= 1)
	{
		return(My_Threads[i].esp);
	} else
	{
		i=0;
		return(My_Threads[i].esp);
	};
};

thread_struct *current_thread()
{
	return(&My_Threads[i]);
};
