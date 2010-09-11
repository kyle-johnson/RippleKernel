OUTPUT_FORMAT("coff-go32")
ENTRY(start)
SECTIONS
{
    .text 0x100000 :	/* 1 meg */
    {
/* kernel code */
	code = .; _code = .; __code = .;
	*(.text)
	. = ALIGN(4096);
    }
    .data :
    {
/* kernel data */
	data = .; _data = .; __data = .;
	*(.data)
	. = ALIGN(4096);
    }
    .bss :
    {
/* kernel BSS */
	bss = .; _bss = .; __bss = .;
	*(.bss)
	*(COMMON) /* GNU C "common" variables */
	. = ALIGN(4096);
    }
    end = .; _end = .; __end = .;
}
