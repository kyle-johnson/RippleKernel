#ifndef __EXCEPTIONS_HEADER
#define __EXCEPTIONS_HEADER

extern isr0;
extern isr1;
extern isr2;
extern isr3;
extern isr4;
extern isr5;
extern isr6;
extern isr7;
extern isr8;
extern isr9;
extern isr10;
extern isr11;
extern isr12;
extern isr13;
extern isr14;
extern isr16;

typedef struct {
	u_long edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u_long gs, fs, es, ds;
	u_long eip, cs, eflags, orig_esp, orig_ss;
} exception_pushed_regs_t;

void page_fault();
void fault(u_long int_num, u_long gs, u_long fs, u_long ds, u_long es, u_long edi, u_long esi, u_long ebp, u_long esp, u_long ebx, u_long edx, u_long ecx, u_long eax, u_long error_code, u_long eip, u_long cs, u_long eflags, u_long orig_esp, u_long orig_ss);
unsigned int panic(char *message);

#endif
