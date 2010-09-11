#include <data_types.h>
#include <mutex.h>
#include <k_printf.h>
#include <tasks.h>
#include <threads.h>
#include <processes.h>
#include <scheduler.h>

extern __total_num_threads;

u_char scheduler_mutex=0;
u_char i=0;
u_char first_time=1;

thread_struct My_Threads[1];
u_long new_esp;
u_long old_esp;

void make_threads()
{
	create_thread(&My_Threads[0], 0, (u_long)&task_1, 0x08, 0x10, 0x08);
//	create_thread(&My_Threads[1], 0, (u_long)&task_2, 0x08, 0x10, 0x08);
//	create_thread(&My_Threads[2], 0, (u_long)&task_1, 0x08, 0x10, 0x08);
//	create_thread(&My_Threads[3], 0, (u_long)&task_2, 0x08, 0x10, 0x08);
//	create_thread(&My_Threads[4], 0, (u_long)&task_1, 0x08, 0x10, 0x08);
//	create_thread(&My_Threads[1], 0, (u_long)&cool_down_thread, 0x08, 0x10, 0x08);
};

void *the_scheduler(u_long esp)
{
	if(first_time == 0)
	{
		//k_printf("\nold_esp: 0x%x\n", old_esp);
		My_Threads[0].esp = esp;
	} else
	{
		first_time = 0;
	};

	return(My_Threads[0].esp);
};

void scheduler()
{
	// lock_mutex_block(&scheduler_mutex);

	if(first_time == 0)
	{
		//k_printf("\nold_esp: 0x%x\n", old_esp);
		My_Threads[0].esp = old_esp;
	} else
	{
		first_time = 0;
	};

	//if(i<__total_num_threads)
	//{
		new_esp = My_Threads[0].esp;
	//} else
	//{
	//	i = 0;
	//	new_esp = My_Threads[i].esp;
	//};
	//k_printf("new_esp: 0x%x\n", new_esp);
	//putc('z');
	// unlock_mutex(&scheduler_mutex);
};
