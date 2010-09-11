[bits 32]
[section .text]
[extern _LINEAR_DATA_SEL]

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
	;push esp

	extern _current_thread
	call _current_thread
	mov [eax], esp		; save esp for current thread

	;mov eax, 0
	;mov gs, eax
	;mov fs, eax
	;mov eax, _LINEAR_DATA_SEL
	;mov es, eax
	;mov ds, eax

	extern _the_scheduler
	call _the_scheduler

	;pop ebx
	mov esp, eax

	mov al, 0x20
	out 0x20, al

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