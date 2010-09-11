[bits 32]
[section .text]

[extern _fault]
[extern _page_fault]
[extern _LINEAR_DATA_SEL]

[global _isr0]
_isr0:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x0
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr1]
_isr1:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x1
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr2]
_isr2:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x2
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr3]
_isr3:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x3
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr4]
_isr4:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x4
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr5]
_isr5:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x5
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr6]
_isr6:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x6
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr7]
_isr7:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x7
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr8]
_isr8:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x8
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr9]
_isr9:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x9
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr10]
_isr10:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0xa
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr11]
_isr11:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0xb
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr12]
_isr12:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0xc
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt

[global _isr13]
_isr13:
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0xd
	push ax
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
	mov ax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
	mov eax, esp
	mov ax, 0x10
	push ax
	mov eax, _fault
	call eax
	cli	; not..
	hlt	; needed 'cause we never get here.. but, it can't hurt
