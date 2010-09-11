#ifndef __EXCEPTIONS_HEADER
#define __EXCEPTIONS_HEADER

void page_fault();
void fault(u_char int_num);
unsigned int panic(char *message);

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

#endif
