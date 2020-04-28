/* linker script */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH =  128K
}

ENTRY(Reset_Handler)

SECTIONS
{
	.text :
	{
		KEEP(*(.vectors)) /* Vector table */
		*(.text*)         /* Program code */
		*(.rodata*)       /* Read only data */
	} > FLASH

	__etext = ALIGN (4);

	.data : AT (__etext)
	{
		__data_start__ = .;
		*(vtable)
		*(.data*)         /* Read-write initialized data */

		. = ALIGN(4);
		__data_end__ = .;

	} > RAM

	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)       /* Read-write zero initialized data */

		. = ALIGN(4);
		__bss_end__ = .;
	} > RAM

	__StackTop = ORIGIN(RAM) + LENGTH(RAM);
}
