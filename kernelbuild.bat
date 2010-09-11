gcc -c test.c -o test.o
rem gcc -c int30.c -o int30.o
rem gcc -c mm.c -o mm.o
gcc -c putchar.c -o putchar.o
gcc -c string.c -o string.o
rem nasm -f aout test.asm -o test1.o
rem nasm -f aout kbd_isr.asm -o kbd_isr.o
rem nasm -f aout rwregs.asm -o rwregs.o
ld -o kernel.bin -T coffkrnl.ld kbd_isr.o test1.o test.o int30.o rwregs.o mm.o putchar.o string.o