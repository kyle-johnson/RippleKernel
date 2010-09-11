;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; These helper functions read
;; and write to certain registers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
[bits 32]

[global _write_cr4] ; this is for implimenting x86-64
_write_cr4:
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; to be implimented ;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global _read_cr4] ; this is for implimenting x86-64
_read_cr4:
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; to be implimented ;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global _write_cr8] ; this is for implimenting x86-64
_write_cr8:
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; to be implimented ;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global _read_cr8] ; this is for implimenting x86-64
_read_cr8:
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; to be implimented ;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[global _read_cr0]
_read_cr0:
	mov eax, cr0
	retn

[global _write_cr0]
_write_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0,  eax
	pop ebp
	retn

[global _read_cr3]
_read_cr3:
	mov eax, cr3
	retn

[global _write_cr3]
_write_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	retn

[global _read_cr2]
_read_cr2:
	mov eax, cr2
	retn

[global _read_cs]
_read_cs:
	mov eax, cs
	retn

[global _read_ds]
_read_ds:
	mov eax, ds
	retn

[global _read_esp]
_read_esp:
	mov eax, esp
	retn

[global _enable_paging]
_enable_paging:
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	jmp .5
.5:
	retn