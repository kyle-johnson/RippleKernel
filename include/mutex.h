#ifndef __MUTEX_HEADER
#define __MUTEX_HEADER

u_char lock_mutex_block(u_char *mutex_pt);
u_char lock_mutex_noblock(u_char *mutex_pt);
void unlock_mutex(u_char *mutex_pt);

#endif
