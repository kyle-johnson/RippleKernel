#ifndef __MUTEX_HEADER
#define __MUTEX_HEADER

u_char lock_mutex_block(u_char *mutex_pt);
#define lock_mutex = lock_mutex_block
u_char lock_mutex_noblock(u_char *mutex_pt);
void unlock_mutex(u_char *mutex_pt);

#endif
