#ifndef __PROCESSES_HEADER
#define __PROCESSES_HEADER

typedef struct {
	u_short id;		// process id, this is assigned when the process is created by the kernel
	u_char process_mutex;
	u_long cr3_handle;	// currently unused

	u_char pl;		// privledge level... 0 or 3
	u_short num_threads;	// how many threads the process has... if it is not 1 or higher, the process should be nuked
} process_struct;

#endif
