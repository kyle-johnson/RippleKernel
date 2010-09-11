[bits 32]
[section .text]

[extern _fault]
[extern _page_fault]
[extern _LINEAR_DATA_SEL]

%macro PUSH_REGS_SETUP_SEGS 1
	pusha
	push es
	push ds
	push fs
	push gs

	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov eax, 0
	mov fs, eax
	mov gs, eax

	;mov eax, esp
	;push eax		; create pointer
	mov eax, %1
	push eax
%endmacro

[global _isr0]
_isr0:
	cli
	PUSH_REGS_SETUP_SEGS 0x0

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr1]
_isr1:
	cli
	PUSH_REGS_SETUP_SEGS 0x1

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr2]
_isr2:
	cli
	PUSH_REGS_SETUP_SEGS 0x2

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr3]
_isr3:
	cli
	PUSH_REGS_SETUP_SEGS 0x3

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr4]
_isr4:
	cli
	PUSH_REGS_SETUP_SEGS 0x4

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr5]
_isr5:
	cli
	PUSH_REGS_SETUP_SEGS 0x5

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr6]
_isr6:
	cli
	PUSH_REGS_SETUP_SEGS 0x6

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr7]
_isr7:
	cli
	PUSH_REGS_SETUP_SEGS 0x7

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr8]
_isr8:
	cli
	PUSH_REGS_SETUP_SEGS 0x8

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr9]
_isr9:
	cli
	PUSH_REGS_SETUP_SEGS 0x9

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr10]
_isr10:
	cli
	PUSH_REGS_SETUP_SEGS 0xa

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr11]
_isr11:
	cli
	PUSH_REGS_SETUP_SEGS 0xb

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr12]
_isr12:
	cli
	PUSH_REGS_SETUP_SEGS 0xc

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr13]
_isr13:
	cli
	PUSH_REGS_SETUP_SEGS 0xd

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr14]
_isr14: ; page fault... this one is IMPORTANT!
	push gs
	push fs
	push es
	push ds
	pusha
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0xe
	push ax
	mov eax, _page_fault
	call eax
	pop ax
	popa
	pop ds
	pop es
	pop fs
	pop gs
	add esp, 4	; drop error code
	iret

[global _isr16]
_isr16:
	cli
	PUSH_REGS_SETUP_SEGS 0x10

	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt
