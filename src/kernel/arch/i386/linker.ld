ENTRY(_start)

SECTIONS
{
    /* Start at physical 1MB */
    . = 0x00100000;

    /* Sections needed before paging is on (Physical) */
    .multiboot ALIGN(4K) : { *(.multiboot) }
	.note.gnu.build-id ALIGN(4K): { *(.note.gnu.build-id) }
    .boot      ALIGN(4K) : { *(.boot)      }

    /* Define the Virtual Offset */
    _kernel_virtual_base = 0xC0000000;
    
    /* Set the location counter to the Higher Half */
    /* This moves the VMA to 3GB + 1MB, but keeps LMA at 1MB */
    . = . + _kernel_virtual_base;

    .text ALIGN(4K) : AT(ADDR(.text) - _kernel_virtual_base)
    {
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - _kernel_virtual_base)
    {
        *(.rodata)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - _kernel_virtual_base)
    {
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - _kernel_virtual_base)
    {
        *(COMMON)
        *(.bss)
    }

    _kernel_end = .;
}