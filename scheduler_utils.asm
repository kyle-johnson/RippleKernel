[bits 32]
[section .text]
[extern _scheduler]
[extern _LINEAR_DATA_SEL]

[global _irq0]
_irq0:
	cli
	push gs				; push segment registers
	push fs
	push es
	push ds
	pusha				; push GP registers

	mov ax, _LINEAR_DATA_SEL	; setup segment registers
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax			; I think gs and fs don't have to be setup for long mode

	mov eax, _scheduler
	call eax				; call the scheduler(it's coded in C)

	mov al, 0x20
	out 0x20, al
	popa				; pop GP registers
	pop ds				; pop segment registers
	pop es
	pop fs
	pop gs
	sti
	iretd