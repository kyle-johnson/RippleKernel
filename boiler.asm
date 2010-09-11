; the "boiler plate" assembly file
; actually, it does quite a bit of essential stuff

%define UNDERBARS
%include "asm.inc"
%include "gdtnasm.inc"

TSS_START equ 0x200000
PAGE_BIT equ 0x80000000

GDT_START equ 0x500

[extern _task_1]
[extern _task_2]

[SECTION .text]
[BITS 32]

[GLOBAL start]
start:
; check to see if paging is enabled
	mov eax, cr0
	and eax, PAGE_BIT
	cmp eax, PAGE_BIT
	jne tea

	; it is, print a 'D' on screen
	mov word [0B8000h],9F44h
	jmp $

; check if data segment linked, located, and loaded properly
tea:
	mov eax,[ds_magic]
	cmp eax,DS_MAGIC
	je ds_ok

; display a blinking white-on-blue 'D' and freeze if we aren't linked properly
	mov word [0B8000h],9F44h
	jmp short $

ds_ok:
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; relocate the GDT to 0x500
	mov esi, gdt
	mov edi, GDT_START
	mov ecx, (gdt_end-gdt)
	cld
	rep movsd
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	lgdt [gdt_ptr]

	mov ax,_LINEAR_DATA_SEL
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov fs,ax
	mov gs,ax
	jmp _LINEAR_CODE_SEL:almost_done

almost_done:

	EXTERN bss, end		; zero the BSS
	mov edi,bss
	mov ecx,end
	sub ecx,edi
	xor eax,eax
	rep stosb

	mov esp,_stack			;set up the _stack

; set up interrupt handlers, then load IDT register
	mov ecx,(idt_end - idt) >> 3		; number of exception handlers
	mov edi,idt
	mov esi,isr0

do_idt:
	mov eax,esi			; EAX=offset of entry point
	mov [edi],ax			; set low 16 bits of gate offset
	shr eax,16
	mov [edi + 6],ax			; set high 16 bits of gate offset
	add edi,8				; 8 bytes/interrupt gate
	add esi,(isr1 - isr0)			; bytes/stub
	loop do_idt

	lidt [idt_ptr]
 
	IMP k_main
	call k_main			;like the 'main' function in C

	jmp $ ;crash

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Multiboot header for GRUB bootloader. This must be in the first 8K
; of the kernel file. We use the aout kludge so it works with ELF,
; DJGPP COFF, Win32 PE, or other formats.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; these are in the linker script file
EXTERN code, bss, end

ALIGN 4
mboot:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM
; aout kludge. These must be PHYSICAL addresses
	dd mboot
	dd code
	dd bss
	dd end
	dd start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; interrupt/exception handlers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

IMP fault

; I shouldn't have to do this!
%macro PUSHB 1
	db 6Ah
	db %1
%endmacro

%macro INTR 1				; (byte offset from start of stub)
isr%1:
	push byte 0			; ( 0) fake error code
	PUSHB %1			; ( 2) exception number
	push gs				; ( 4) push segment registers
	push fs				; ( 6)
	push es				; ( 8)
	push ds				; ( 9)
	pusha				; (10) push GP registers
		mov ax,_LINEAR_DATA_SEL	; (11) put known-good values...
		mov ds,eax		; (15) ...in segment registers
		mov es,eax		; (17)
		mov fs,eax		; (19)
		mov gs,eax		; (21)
		mov eax,esp		; (23)
		push eax		; (25) push pointer to regs_t
.1:
; setvect() changes the operand of the CALL instruction at run-time,
; so we need its location = 27 bytes from start of stub. We also want
; the CALL to use absolute addressing instead of EIP-relative, so:
			mov eax,fault	; (26)
			call eax	; (31)
			jmp all_ints	; (33)
%endmacro				; (38)

%macro INTR_EC 1
isr%1:
	nop				; error code already pushed
	nop				; nop+nop=same length as push byte
	PUSHB %1			; ( 2) exception number
	push gs				; ( 4) push segment registers
	push fs				; ( 6)
	push es				; ( 8)
	push ds				; ( 9)
	pusha				; (10) push GP registers
		mov ax,_LINEAR_DATA_SEL	; (11) put known-good values...
		mov ds,eax		; (15) ...in segment registers
		mov es,eax		; (17)
		mov fs,eax		; (19)
		mov gs,eax		; (21)
		mov eax,esp		; (23)
		push eax		; (25) push pointer to regs_t
.1:
; setvect() changes the operand of the CALL instruction at run-time,
; so we need its location = 27 bytes from start of stub. We also want
; the CALL to use absolute addressing instead of EIP-relative, so:
			mov eax,fault	; (26)
			call eax	; (31)
			jmp all_ints	; (33)
%endmacro				; (38)

; the vector within the stub (operand of the CALL instruction)
; is at (isr0.1 - isr0 + 1)

all_ints:
	pop eax
	popa				; pop GP registers
	pop ds				; pop segment registers
	pop es
	pop fs
	pop gs
	add esp,8			; drop exception number and error code
	iret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; name:			getvect
; action:		reads interrupt vector
; in:			[EBP + 12] = vector number
; out:			vector stored at address given by [EBP + 8]
; modifies:		EAX, EDX
; minimum CPU:		'386+
; notes:		C prototype:
;			typedef struct
;			{	unsigned access_byte, eip; } vector_t;
;			getvect(vector_t *v, unsigned vect_num);
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXP getvect
	push ebp
		mov ebp,esp
		push esi
		push ebx
			mov esi,[ebp + 8]

; get access byte from IDT[i]
			xor ebx,ebx
			mov bl,[ebp + 12]
			shl ebx,3
			mov al,[idt + ebx + 5]
			mov [esi + 0],eax

; get handler address from stub
			mov eax,isr1
			sub eax,isr0	; assume stub size < 256 bytes
			mul byte [ebp + 12]
			mov ebx,eax
			add ebx,isr0
			mov eax,[ebx + (isr0.1 - isr0 + 1)]
			mov [esi + 4],eax
		pop ebx
		pop esi
	pop ebp
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; name:			setvect
; action:		writes interrupt vector
; in:			[EBP + 12] = vector number,
;			vector stored at address given by [EBP + 8]
; out:			(nothing)
; modifies:		EAX, EDX
; minimum CPU:		'386+
; notes:		C prototype:
;			typedef struct
;			{	unsigned access_byte, eip; } vector_t;
;			getvect(vector_t *v, unsigned vect_num);
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXP setvect
	push ebp
		mov ebp,esp
		push esi
		push ebx
			mov esi,[ebp + 8]

; store access byte in IDT[i]
			mov eax,[esi + 0]
			xor ebx,ebx
			mov bl,[ebp + 12]
			shl ebx,3
			mov [idt + ebx + 5],al

; store handler address in stub
			mov eax,isr1
			sub eax,isr0	; assume stub size < 256 bytes
			mul byte [ebp + 12]
			mov ebx,eax
			add ebx,isr0
			mov eax,[esi + 4]
			mov [ebx + (isr0.1 - isr0 + 1)],eax
		pop ebx
		pop esi
	pop ebp
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; interrupt/exception stubs
; *** CAUTION: these must be consecutive, and must all be the same size.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	INTR 0		; zero divide (fault)
	INTR 1		; debug/single step
	INTR 2		; non-maskable interrupt (trap)
	INTR 3		; INT3 (trap)
	INTR 4		; INTO (trap)
	INTR 5		; BOUND (fault)
	INTR 6		; invalid opcode (fault)
	INTR 7		; coprocessor not available (fault)
	INTR_EC 8	; double fault (abort w/ error code)
	INTR 9		; coproc segment overrun (abort; 386/486SX only)
	INTR_EC 0Ah	; bad TSS (fault w/ error code)
	INTR_EC 0Bh	; segment not present (fault w/ error code)
	INTR_EC 0Ch	; _stack fault (fault w/ error code)
	INTR_EC 0Dh	; GPF (fault w/ error code)
	INTR_EC 0Eh	; page fault
	INTR 0Fh	; reserved
	INTR 10h	; FP exception/coprocessor error (trap)
	INTR 11h	; alignment check (trap; 486+ only)
	INTR 12h	; machine check (Pentium+ only)
	INTR 13h
	INTR 14h
	INTR 15h
	INTR 16h
	INTR 17h
	INTR 18h
	INTR 19h
	INTR 1Ah
	INTR 1Bh
	INTR 1Ch
	INTR 1Dh
	INTR 1Eh
	INTR 1Fh

; isr20 through isr2F are hardware interrupts. The 8259 programmable
; interrupt controller (PIC) chips must be reprogrammed to make these work.
	INTR 20h	; IRQ 0/timer interrupt
	INTR 21h	; IRQ 1/keyboard interrupt
	INTR 22h
	INTR 23h
	INTR 24h
	INTR 25h
	INTR 26h	; IRQ 6/floppy interrupt
	INTR 27h
	INTR 28h	; IRQ 8/real-time clock interrupt
	INTR 29h
	INTR 2Ah
	INTR 2Bh
	INTR 2Ch
	INTR 2Dh	; IRQ 13/math coprocessor interrupt
	INTR 2Eh	; IRQ 14/primary ATA ("IDE") drive interrupt
	INTR 2Fh	; IRQ 15/secondary ATA drive interrupt

; syscall software interrupt
	INTR 30h

; the other 207 vectors are undefined

%assign i 31h
%rep (0FFh - 30h)

	INTR i

%assign i (i + 1)
%endrep



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[SECTION .data]
ds_magic:		;this is so we know if our data segment was linked corectly
	dd DS_MAGIC	; ^

TSS_SIZE	equ	108

gdt:			;our descriptors
; NULL descriptor
	dw 0		; limit 15:0
	dw 0		; base 15:0
	db 0		; base 23:16
	db 0		; type
	db 0		; limit 19:16, flags
	db 0		; base 31:24

[global _LINEAR_DATA_SEL]
_LINEAR_DATA_SEL	equ	$-gdt
	dw 0FFFFh
	dw 0
	db 0
	db 92h		; present, ring 0, data, expand-up, writable
	db 0CFh		; page-granular (4 gig limit), 32-bit
	db 0
[global _LINEAR_CODE_SEL]
_LINEAR_CODE_SEL	equ	$-gdt
	dw 0FFFFh
	dw 0
	db 0
	db 9Ah		; present,ring 0,code,non-conforming,readable
	db 0CFh		; page-granular (4 gig limit), 32-bit
	db 0
[global _TSS_ENTRY_0]
_TSS_ENTRY_0		equ	$-gdt
	desc TSS_START, TSS_SIZE, D_TSS	
[global _TSS_ENTRY_1]
_TSS_ENTRY_1		equ	$-gdt
	desc TSS_START+TSS_SIZE, TSS_SIZE, D_TSS
[global _TSS_ENTRY_2]
_TSS_ENTRY_2		equ	$-gdt
	desc TSS_START+TSS_SIZE, TSS_SIZE, D_TSS

gdt_end:

gdt_ptr:
	dw gdt_end - gdt - 1
	dd 0x500

; 256 ring 0 interrupt gates

idt:
%rep 256
	dw 0				; offset 15:0
	dw _LINEAR_CODE_SEL		; selector
	db 0				; (always 0 for interrupt gates)
	db 8Eh				; present,ring 0,'386 interrupt gate
	dw 0				; offset 31:16
%endrep
idt_end:

idt_ptr:
	dw idt_end - idt - 1			; IDT limit
	dd idt				; linear adr of IDT

[global _stack]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[SECTION .bss]
	resb 1024
	_stack:				;the _stack