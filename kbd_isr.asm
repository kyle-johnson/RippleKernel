[BITS 32]
[SECTION .text]

;;;;;;;;;;;;;;;;;;;;; isr for IRQ1(the keyboard) ;;;;;;;;;;;;;;;;;;
[global _kbd_isr]
_kbd_isr:				; just "kbd_isr" in C
	cli
	mov esi, pKbdRawIn		;pointer to the raw kbd buffer
	xor eax, eax		;zero eax
	in al, 60h			;get 1 byte from the keyboard

	mov ebx, KbdRawCnt	;check if the raw buffer is full
	cmp ebx, 20h
	je kbd_isr_finish		;if it is, exit
	mov [esi], al		;store al(byte from the kbd) in the raw buffer
	inc dword [KbdRawCnt]
	inc esi
	cmp esi, KbdRawBuf+20h	;are we past the end of the buffer?
	jb kbd_isr_finish
	mov esi, KbdRawBuf

kbd_isr_finish:
	mov eax, [esi]
	mov [pKbdRawIn], eax
	mov al, 0x20
	out 0x20, al
	sti
	pop eax
	popa				; pop GP registers
	pop ds				; pop segment registers
	pop es
	pop fs
	pop gs
	add esp,8			; drop exception number and error code
	iret

;;;;;;;;;;;;;;;;;;;;;;;; keyboard code for retriving a byte from the raw buffer ;;;;;;;;;;;;;;;;;;;;;;;;

; this will return 0 in eax if no code is found
; or the code in al
kbd_getraw:
	cli			;disable ints(neccasary so that the keyboard ISR doesn't mess us up)
	mov esi, pKbdRawOut	;get pointer to next charactor to come out
	mov eax, KbdRawCnt	;see if there are any bytes
	cmp eax, 0
	je kbd_getraw_done	;nope, leave 0 in eax
	dec dword [KbdRawCnt]	;yes, make KbdRawCnt correct
	xor eax, eax		;zero eax
	mov eax, [esi]
	inc esi
	cmp esi, KbdRawBuf+0x20
	jb kbd_getraw_done
	mov esi, KbdRawBuf

kbd_getraw_done:
	mov [pKbdRawOut], esi
	sti			;gotta enable ints again
	ret

[SECTION .data]
%include "kbd_scancodes_set2_US.inc"

KbdRawBuf:
	times 32 DB 0	;32-byte raw buffer

KbdRawCnt:
	DD 0

pKbdRawIn:
	DD KbdRawBuf

pKbdRawOut:
	DD KbdRawBuf


KbdBuffer:
	times 64 DD 0	;64 DWords for the translated kbd buffer

KbdCnt:
	DD 0

pKbdIn:
	DD KbdBuffer

pKbdOut:
	DD KbdBuffer