# makefile for Ripple
CC = gcc
CFLAGS = -O2 -fno-builtins -Ic:\djgpp\myos\include -c

NASM = nasm
NASMFLAGS = -f aout -i include

LD = ld
LDFLAGS = -T binkrnl.ld

UDI_OBJS = udi\blah.o

KERN_OBJS = kernel.o boiler.o mm.o kbd_isr.o k_printf.o string.o putchar.o rwRegs.o real_time_clock.o rtc.o floppy.o tasks.o TSS.o tss-management.o mutex.o descriptor.o scheduler.o mp.o vga.o vga_utils.o ports.o exceptions.o exceptions_utils.o rand.o idt.o scheduler_utils.o cpu.o threads.o processes.o vbe3.o phys_mm.o malloc.o
KERN_NAME = kernel.bin

all : $(KERN_NAME)

# assemble all the ASM files
%.o : %.asm
	$(NASM) $(NASMFLAGS) $<

#compile all the C files
%.o : %.c
	$(CC) $(CFLAGS) $<

#make all the UDI stuff(it has it's own Makefile
$(UDI_OBJS):
	cd udi
	make
	cd ..

# link them all together
$(KERN_NAME) : $(KERN_OBJS) $(UDI_OBJS)
	$(LD)  $(LDFLAGS) -o $(KERN_NAME) $(KERN_OBJS) $(UDI_OBJS)

clean:
	del *.o

map:
	$(LD)  $(LDFLAGS) -Map os.map -o $(KERN_NAME) $(KERN_OBJS) $(UDI_OBJS)