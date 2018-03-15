OUTPUT_FORMAT(elf32-i386)
ENTRY(load)

SECTIONS
{
    /* 3GB and 1MB into mem */
    . = 0xC0100000;

    k_mem_start = .;

    .text ALIGN (0x1000) : AT(ADDR(.text) - 0xC0000000) {
        text = .;
        *(.text)
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - 0xC0000000) {
        *(.rodata*)
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000) {
        *(.data)
    }

    .bss ALIGN (0x1000) : AT(ADDR(.bss) - 0xC0000000) {
        bssstart = .;
        *(.COMMON)
        *(.bss)
        bssend = .;
    }

    k_mem_end = .;

}
