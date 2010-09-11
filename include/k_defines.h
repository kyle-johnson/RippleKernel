#ifndef K_HEADER
#define K_HEADER

#define	PIC1		0x20
#define	PIC2		0xA0
#define	PIC1_COMMAND	PIC1
#define	PIC1_DATA	(PIC1+1)
#define	PIC2_COMMAND	PIC2
#define	PIC2_DATA	(PIC2+1)
#define	PIC_EOI		0x20

#define	ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define	ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define	ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define	ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define	ICW1_INIT	0x10		/* Initialization - required! */

#define	ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define	ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define	ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define	ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define	ICW4_SFNM	0x10		/* Special fully nested (not) */



#define WHITE_TXT 0x7
#define BLUE_TXT 0x09

#define UCHAR unsigned char
#define USHORT unsigned short

/* the code for setvect() and getvect() in
TEST.ASM depends on the layout of this structure */
typedef struct
{
	unsigned access_byte, eip;
} vector_t;

// IMPORTS
// from TEST.ASM
void getvect(vector_t *v, unsigned vect_num);
void setvect(vector_t *v, unsigned vect_num);

// needed for IRQ enableing
USHORT irqs;

typedef struct
{
	// pushed by pusha
	unsigned edi, esi, ebp, esp, ebx, edx, ecx, eax;

	// pushed separately
	unsigned ds, es, fs, gs;
	unsigned which_int, err_code;

	// pushed by exception. Exception may also push err_code.
	// user_esp and user_ss are pushed only if a privilege change occurs.
	unsigned eip, cs, eflags, user_esp, user_ss;
} regs_t;

// from test.c
inline void outportb(unsigned int port, unsigned char value);
inline void outportw(unsigned int port, unsigned int value);
inline unsigned char inportb(unsigned short port);

void remap_pics(int pic1, int pic2);
void unmask_irq(int irq);
void mask_irq(int irq);

void fault(regs_t *regs);
unsigned int panic(char *message);

void k_clear_screen();
void update_cursor(int row, int col);
unsigned int k_load();

void enable_ints();
void disable_ints();

// int 30 routine
void int30(regs_t *regs);

#endif
