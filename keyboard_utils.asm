[BITS 32]
[SECTION .text]

[extern _LINEAR_DATA_SEL]

;;;;;;;;;;;;;;;;;;;;; isr for IRQ1(the keyboard) ;;;;;;;;;;;;;;;;;;
[global _kbd_isr_2]
_kbd_isr_2:
	;cli
	push gs
	push fs
	push es
	push ds
	pusha

	mov eax, _LINEAR_DATA_SEL
	mov ds, eax
	mov es, eax
	mov eax, 0
	mov fs, eax
	mov gs, eax

	extern _store_scancode
	call _store_scancode

	mov al, 0x20
	out 0x20, al
	popa				; pop GP registers
	pop ds				; pop segment registers
	pop es
	pop fs
	pop gs
	;sti
	iretd