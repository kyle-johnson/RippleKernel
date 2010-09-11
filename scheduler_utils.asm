[bits 32]
[section .text]
[extern _scheduler]
[extern _LINEAR_DATA_SEL]
[extern _new_esp]
[extern _old_esp]

[global _irq0]
_irq0:
	cli
	push gs				; push segment registers
	push fs
	push es
	push ds
	pushad

	mov eax, esp			; this way the _scheduler routine can get the old esp
	mov [_old_esp], eax

	mov eax, _LINEAR_DATA_SEL	; setup segment registers
	mov ds, eax
	mov es, eax
	mov eax, 0
	mov fs, eax
	mov gs, eax			; I think gs and fs don't have to be setup for long mode

	mov eax, _scheduler
	call eax				; call the scheduler(it's coded in C)

	mov al, 0x20
	out 0x20, al

	mov esp, [_new_esp]

	popad
	pop ds				; pop segment registers
	pop es
	pop fs
	pop gs
	sti
	iretd