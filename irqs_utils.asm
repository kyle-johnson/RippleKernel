[bits 32]
[extern _irq_watcher_helper]
[extern _LINEAR_DATA_SEL]

%macro IRQ_WATCHER_1 1
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

	mov eax, %1
	push eax

	extern _irq_watcher_helper
	call _irq_watcher_helper

	pop eax

	mov al, 0x20
	out 0x20, al

	pop gs				; pop segment registers
	pop fs
	pop ds
	pop es
	popa				; pop GP registers
	iretd
%endmacro

%macro IRQ_WATCHER_2 1
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

	mov eax, %1
	push eax

	extern _irq_watcher_helper
	call _irq_watcher_helper

	pop eax

	mov al, 0x20
	out 0xA0, al
	mov al, 0x20
	out 0x20, al

	pop gs				; pop segment registers
	pop fs
	pop ds
	pop es
	popa				; pop GP registers
	iretd
%endmacro

[global _irq_watcher_1]
_irq_watcher_1:
	IRQ_WATCHER_1 0x1

[global _irq_watcher_2]
_irq_watcher_2:
	IRQ_WATCHER_1 0x2

[global _irq_watcher_3]
_irq_watcher_3:
	IRQ_WATCHER_1 0x3

[global _irq_watcher_4]
_irq_watcher_4:
	IRQ_WATCHER_1 0x4

[global _irq_watcher_5]
_irq_watcher_5:
	IRQ_WATCHER_1 0x5

[global _irq_watcher_6]
_irq_watcher_6:
	IRQ_WATCHER_1 0x6

[global _irq_watcher_7]
_irq_watcher_7:
	IRQ_WATCHER_1 0x7

[global _irq_watcher_8]
_irq_watcher_8:
	IRQ_WATCHER_2 0x8

[global _irq_watcher_9]
_irq_watcher_9:
	IRQ_WATCHER_2 0x9

[global _irq_watcher_10]
_irq_watcher_10:
	IRQ_WATCHER_2 0xA

[global _irq_watcher_11]
_irq_watcher_11:
	IRQ_WATCHER_2 0xB

[global _irq_watcher_12]
_irq_watcher_12:
	IRQ_WATCHER_2 0xC

[global _irq_watcher_13]
_irq_watcher_13:
	IRQ_WATCHER_2 0xD

[global _irq_watcher_14]
_irq_watcher_14:
	IRQ_WATCHER_2 0xE

[global _irq_watcher_15]
_irq_watcher_15:
	IRQ_WATCHER_2 0xF
