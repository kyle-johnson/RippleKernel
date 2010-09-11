#ifndef __SCHEDULER_HEADER
#define __SCHEDULER_HEADER

typedef struct {
	u_long *ptrToNextBlock;		// 0 if this is the end of the list
	thread_struct *ptrToThread;
	process_struct *ptrToProcess;
} thread_info_block;

void scheduler();

extern irq0;

#endif
