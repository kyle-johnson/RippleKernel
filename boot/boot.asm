[org 0x7c00]
[bits 16]

jmp short boot_start
nop

boot_start:
	cli
	mov ax, cs	; setup segs
	mov es, ax
	mov ds, ax
	mov ss, ax
	xor sp, sp
	push dx		; save drive #
	jmp $		; hang

times 510-($-$$) db 0	; pad as needed
dw 0xAA55		; boot sig

times 0x168000-($-$$) db 0	; makes this into a usable floppy image