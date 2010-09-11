; the "boiler plate" assembly file
; actually, it does quite a bit of essential stuff

%define UNDERBARS
%include "asm.inc"
%include "gdtnasm.inc"

TSS_START equ 0x200000
PAGE_BIT equ 0x80000000

GDT_START equ 0x500
IDT_START equ 0x600

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
	cli
	hlt

; check if data segment linked, located, and loaded properly
tea:
	mov eax,[ds_magic]
	cmp eax,DS_MAGIC
	je ds_ok

; display a blinking white-on-blue 'D' and freeze if we aren't linked properly
	mov word [0B8000h],9F44h
	cli
	hlt

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
	mov ax, 0x0
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

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; relocate the IDT to 0x600
	mov esi, idt
	mov edi, IDT_START
	mov ecx, (idt_end-idt)
	cld
	rep movsd
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	lidt [idt_ptr]
 
	EXTERN _k_main
	call _k_main			;like the 'main' function in C

	cli
	hlt

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
gdt_end:

gdt_ptr:
	dw gdt_end - gdt - 1
	dd 0x500


idt:
%rep 100	; enough descriptors to get past the block of IRQs and then some(up to int 0x64)
	dw 0x0
	dw _LINEAR_CODE_SEL
	dw 0x8E00
	dw 0x0
%endrep
idt_end:

idt_ptr:
	dw idt_end - idt - 1			; IDT limit
	dd 0x600				; linear adr of IDT

[global _stack]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[SECTION .bss]
	resb 1024
	_stack:				;the _stack