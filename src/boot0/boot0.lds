SECTIONS
{
	. = 0x7220;
	.text : { *(.text) }
	. = ALIGN(0x10);
	.data : { *(.data) }

	.bss : { 
		_bss = .;		/* �������bss�� */
		*(.bss) *(.COMMON)
		_bss_end = .;
	}

}