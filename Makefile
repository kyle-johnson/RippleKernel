# makefile for (codename)Spotlight
CC = gcc
CFLAGS = -fno-builtins -Ic:\djgpp\myos\include -c

NASM = nasm
NASMFLAGS = -f aout -i include

LD = ld
LDFLAGS = -T coffkrnl.ld

KERN_OBJS = kernel.o boiler.o mm.o int30.o kbd_isr.o k_printf.o string.o putchar.o rwRegs.o real_time_clock.o rtc.o floppy.o tasks.o TSS.o tss-management.o mutex.o descriptor.o
KERN_NAME = kernel.bin

all : $(KERN_NAME)

# assemble all the ASM files
%.o : %.asm
	$(NASM) $(NASMFLAGS) $<

#compile all the C files
%.o : %.c
	$(CC) $(CFLAGS) $<

# link them all together
$(KERN_NAME) : $(KERN_OBJS)
	$(LD)  $(LDFLAGS) -o $(KERN_NAME) $(KERN_OBJS)