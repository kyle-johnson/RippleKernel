%include "gdtnasm.inc"
[bits 32]
[extern _TSS_ENTRY_0]
[extern _TSS_ENTRY_1]

[global _write_ltr]
_write_ltr:
	push ebp
	mov ebp, esp
	mov eax, [ebp+4]
	ltr ax
	pop ebp
	retn

[global _load_first_ltr]
_load_first_ltr:
	mov ax, _TSS_ENTRY_0
	ltr ax
	retn

[global _jmp_tss_1]
_jmp_tss_1:
	jmp _TSS_ENTRY_1 : 0
