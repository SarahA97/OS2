SECTIONS
{
	. = 0x8000;

	.text ALIGN(4096) : {
		*(.boot)
		*(.text)
	}

	.data ALIGN(4096) : {
		*(.data)
		*(.rodata)
	}

	__bss_start = .;
	.bss ALIGN(4096) : {
		*(.bss)
	}
	__bss_end = .;

	. = ALIGN(4096);
	__end = .;
}
