/////////////////////////////////////////////////////////////////////
// Processes own threads. A process must have at least 1
// thread to exist(unless the process'es mutex is locked) or
// that process is killed. Each process has its' own
// structure(process_struct). Processes cannot have more
// than 1 address space. The threads that the process owns
// share that address space. Note that if the process is PL 0,
// it doesn't actually have it's own address space, but shares
// one with all the other ring 0 processes(except for the kernel).
////////////////////////////////////////////////////////////////////
#include <data_types.h>
#include <processes.h>