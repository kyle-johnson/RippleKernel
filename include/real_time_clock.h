#ifndef REAL_TIME_CLOCK_HEADER
#define REAL_TIME_CLOCK_HEADER

volatile unsigned int amount_of_ticks;

void real_time_clock_ISR(regs_t *regs);
inline void sleep(unsigned int ticks);
void get_time_int(int hr, int min, int sec);
void get_time_str(u_char no_leading_zero, char *hr, char *min, char *sec);

#endif
