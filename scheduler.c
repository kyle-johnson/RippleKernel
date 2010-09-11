#include <data_types.h>
#include <mutex.h>
#include <k_printf.h>
#include <tasks.h>
#include <threads.h>
#include <processes.h>
#include <keyboard.h>
#include <scheduler.h>

extern __total_num_threads;

u_char scheduler_mutex=0;
u_char i=0;

thread_struct My_Threads[1];

thread_struct *__current_thread = (thread_struct *)0;
thread_info_block *__current_tib = (thread_info_block *)0;

void make_threads()
{
	thread_struct *blah;
//	create_thread(&My_Threads[0], 0, (u_long)&cool_down_thread, 0x08, 0x10, 0x08, 0);
	create_thread(malloc(sizeof(thread_struct)), 0, (u_long)&cool_down_thread, 0x08, 0x10, 0x08, 0);
	blah = malloc(sizeof(thread_struct));
	create_thread(blah, 0, (u_long)&setup_keyboard, 0x08, 0x10, 0x08, 0);
//	create_thread(malloc(sizeof(thread_struct)), 0, (u_long)&setup_keyboard, 0x08, 0x10, 0x08, 0);
	k_printf("Done making threads.\n");
};

void *the_scheduler(u_long esp)
{
	__current_tib = __current_tib->ptrToNextBlock;
	__current_thread = __current_tib->ptrToThread;

	return(__current_thread->esp);
};

void set_pit_interval(u_short interval_in_hz)
{
	u_short calculated;
	calculated = 1193182 / interval_in_hz;
	outportb(0x43, 0x36);
	outportb(0x40, calculated & 0xFF);
	outportb(0x40, calculated >> 8);
};

thread_struct *current_thread()
{
	return(__current_thread);
};
