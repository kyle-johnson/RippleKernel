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
	popa				; pop GP registers
	pop ds				; pop segment registers
	pop es
	pop fs
	pop gs
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
	popa				; pop GP registers
	pop ds				; pop segment registers
	pop es
	pop fs
	pop gs
	iretd
%endmacro

_irq_watcher_1:
	IRQ_WATCHER_1 0x1

_irq_watcher_2:
	IRQ_WATCHER_1 0x2

_irq_watcher_3:
	IRQ_WATCHER_1 0x3

_irq_watcher_4:
	IRQ_WATCHER_1 0x4

_irq_watcher_5:
	IRQ_WATCHER_1 0x5

_irq_watcher_6:
	IRQ_WATCHER_1 0x6

_irq_watcher_7:
	IRQ_WATCHER_1 0x7

_irq_watcher_8:
	IRQ_WATCHER_2 0x8

_irq_watcher_9:
	IRQ_WATCHER_2 0x9

_irq_watcher_10:
	IRQ_WATCHER_2 0xA

_irq_watcher_11:
	IRQ_WATCHER_2 0xB

_irq_watcher_12:
	IRQ_WATCHER_2 0xC

_irq_watcher_13:
	IRQ_WATCHER_2 0xD

_irq_watcher_14:
	IRQ_WATCHER_2 0xE

_irq_watcher_15:
	IRQ_WATCHER_2 0xF
