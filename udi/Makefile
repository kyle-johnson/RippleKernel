# makefile for Ripple
CC = gcc
CFLAGS = -O2 -fno-builtins -Ic:\djgpp\myos\include -Ic:\djgpp\myos\udi\include -c

NASM = nasm
NASMFLAGS = -f aout -i ..\include -i include

UDI_OBJS = blah.o

all : $(UDI_OBJS)

#compile all the C files
%.o : %.c
	$(CC) $(CFLAGS) $<