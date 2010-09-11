;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; These helper functions read
;; and write to certain registers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[bits 32]

[global _read_cr0]
_read_cr0:
	mov eax, cr0
	mov edx, eax
	shr edx, 16	; the return value is CR0
	retn

[global _write_cr0]
_write_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+4]
	mov cr0,  eax
	pop ebp
	retn

[global _read_cr3]
_read_cr3:
	mov eax, cr3
	mov edx, eax
	shr edx, 16	; the return value is CR3
	retn

[global _write_cr3]
_write_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+4]
	mov cr3, eax
	pop ebp
	retn

[global _read_cr2]
_read_cr2:
	mov eax, cr2
	mov edx, eax
	shr edx, 16	; the return value is CR2
	retn