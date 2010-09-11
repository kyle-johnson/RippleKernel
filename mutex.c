/************************************************************
  Mutexes are esential. Not just on multiproccessor
  systems, but on singal porccesser systems. For
  instance, on task might malloc some memory, but
  before the memory map can be updated, another
  task mallocs some memory. Obviously, that would
  create a huge problem(likely a crash). Mutexes may
  be used to keep problems like that from happening.
************************************************************/
#include <mutex.h>
