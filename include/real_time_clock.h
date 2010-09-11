#ifndef REAL_TIME_CLOCK_HEADER
#define REAL_TIME_CLOCK_HEADER

volatile unsigned int amount_of_ticks;

void real_time_clock_ISR(regs_t *regs);
void enable_real_time_clock();
void disable_real_time_clock();
inline void sleep(unsigned int ticks);

#endif
