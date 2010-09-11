#ifndef __THREADS_HEADER
#define __THREADS_HEADER

typedef struct {
	u_short id;		// thread's "personal" id, assigned to it by the kernel
	u_short process_id;	// the id of the process that owns the thread

	u_char executing;		// 0 = no, 1 = yes
	u_char thread_mutex;	// the mutex of the process that owns the thread overrides the thread's mutex if the process mutex is locked
	u_char sleeping;		// 0 = no, 1 = yes
	u_long sleep_ticks;	// the process can't sleep for more than 4,194,303 seconds... should be plenty :)
	u_long stack[255];		// stack for this thread(1024 bytes long)
} thread_struct;

void create_thread(thread_struct *new_thread, u_short process_id, u_long eip, u_long ds, u_long cs, u_long ss);

#endif
