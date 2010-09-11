#ifndef __IRQS_HEADER
#define __IRQS_HEADER

#define	PIC1		0x20
#define	PIC2		0xA0
#define	PIC1_COMMAND	PIC1
#define	PIC1_DATA	(PIC1+1)
#define	PIC2_COMMAND	PIC2
#define	PIC2_DATA	(PIC2+1)
#define	PIC_EOI		0x20

#define	ICW1_ICW4	0x01		// ICW4 (not) needed
#define	ICW1_SINGLE	0x02		// Single (cascade) mode
#define	ICW1_INTERVAL4	0x04		// Call address interval 4 (8)
#define	ICW1_LEVEL	0x08		// Level triggered (edge) mode
#define	ICW1_INIT	0x10		// Initialization - required!

#define	ICW4_8086		0x01	// 8086/88 (MCS-80/85) mode
#define	ICW4_AUTO		0x02	// Auto (normal) EOI
#define	ICW4_BUF_SLAVE		0x08	// Buffered mode/slave
#define	ICW4_BUF_MASTER	0x0C	// Buffered mode/master
#define	ICW4_SFNM		0x10	// Special fully nested (not)

//////////////////////////////////////////////////////////////////////

extern _irq_watcher_1;
extern _irq_watcher_2;
extern _irq_watcher_3;
extern _irq_watcher_4;
extern _irq_watcher_5;
extern _irq_watcher_6;
extern _irq_watcher_7;
extern _irq_watcher_8;
extern _irq_watcher_9;
extern _irq_watcher_10;
extern _irq_watcher_11;
extern _irq_watcher_12;
extern _irq_watcher_13;
extern _irq_watcher_14;
extern _irq_watcher_15;

//////////////////////////////////////////////////////////////////////

u_char watch_irq(u_char irq_number, u_long *key);
void release_irq(u_char irq_number, u_long *key);
void irq_watcher_helper(u_long irq_number);
void remap_pics(u_char pic1, u_char pic2);
void unmask_irq(u_char irq);
void mask_irq(u_char irq);

#endif
