SECTIONS
{
. = 0x80010000;
.text : { *(.text) *(.rodata) }
. = ALIGN(0x10);
.data : { *(.data) }
_bss = .;		/* ”√¿¥«Âø’bss∂Œ */
.bss : {
	*(.bss) *(.COMMON)
}
_bss_end = .;
}