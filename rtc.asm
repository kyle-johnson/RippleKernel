[bits 32]

[global _enable_rtc]
_enable_rtc:
	mov al, 0x0B
	out 0x70, al
	in al, 0x71
	or al, 0x40
	out 0x71, al
	ret

[global _disable_rtc]
_disable_rtc:
	mov al, 0x0B
	out 0x70, al
	in al, 0x71
	and al, 0xBF
	out 0x71, al
	ret