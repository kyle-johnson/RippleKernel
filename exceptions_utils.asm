[bits 32]
[section .text]

[extern _fault]

[global int0]
int0:
	mov al, 0x0
	push al
	mov eax, _fault
	call eax

[global int1]
int1:
	mov al, 0x1
	push al
	mov eax, _fault
	call eax

[global int2]
int2:
	mov al, 0x2
	push al
	mov eax, _fault
	call eax

[global int3]
int3:
	mov al, 0x3
	push al
	mov eax, _fault
	call eax

[global int4]
int4:
	mov al, 0x4
	push al
	mov eax, _fault
	call eax

[global int5]
int5:
	mov al, 0x5
	push al
	mov eax, _fault
	call eax

[global int6]
int6:
	mov al, 0x6
	push al
	mov eax, _fault
	call eax

[global int7]
int7:
	mov al, 0x7
	push al
	mov eax, _fault
	call eax

[global int8]
int8:
	mov al, 0x8
	push al
	mov eax, _fault
	call eax

[global int9]
int9:
	mov al, 0x9
	push al
	mov eax, _fault
	call eax

[global int10]
int10:
	mov al, 0xa
	push al
	mov eax, _fault
	call eax

[global int11]
int11:
	mov al, 0xb
	push al
	mov eax, _fault
	call eax

[global int12]
int12:
	mov al, 0xc
	push al
	mov eax, _fault
	call eax

[global int13]
int13:
	mov al, 0xd
	push al
	mov eax, _fault
	call eax

[global int14]
int14: ; page fault... this one is IMPORTANT!
	mov al, 0xe
	push al
	mov eax, _fault
	call eax

[global int15]
int15:
	mov al, 0xf
	push al
	mov eax, _fault
	call eax

[global int16]
int16:
	mov al, 0x10
	push al
	mov eax, _fault
	call eax
