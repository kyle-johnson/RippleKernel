[bits 32]
[section .text]
[extern _scheduler]
[extern _LINEAR_DATA_SEL]
[extern _new_esp]
[extern _old_esp]

[global _irq0]
_irq0:
	cli
	push eax
	push ebx
	push ecx
	push edx
	push ebp
	push edi
	push esi
	push es
	push ds
	push fs
	push gs
	push esp

	mov eax, 0
	mov gs, eax
	mov fs, eax
	mov eax, _LINEAR_DATA_SEL
	mov es, eax
	mov ds, eax

	extern _the_scheduler
	call _the_scheduler

	mov al, 0x20
	out 0x20, al

	pop ebx
	mov esp, eax
	pop gs
	pop fs
	pop ds
	pop es
	pop esi
	pop edi
	pop ebp
	pop edx
	pop ecx
	pop ebx
	pop eax
	sti
	iretd
















_irq0_old:
	cli

	push es
	push ds
	push gs				; push segment registers
	push fs
	pushad

	;mov eax, esp			; this way the _scheduler routine can get the old esp
	mov [_old_esp+0], esp

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

	mov esp, [_new_esp+0]

	popad

	pop fs
	pop gs
	pop ds				; pop segment registers
	pop es
	sti
	iretd