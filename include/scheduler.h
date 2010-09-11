#ifndef __SCHEDULER_HEADER
#define __SCHEDULER_HEADER

void scheduler();
void *the_scheduler(u_long esp);
void make_threads();

extern irq0;

#endif
