ENTRY(_start)

SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . = 0x8000;
    __start = .;
    
    /* Boot sections */
    .text.boot : {
        KEEP(*(.text.boot))
    }
    
    /* Code section */
    .text : {
        *(.text)
    }
    
    /* Read-only data */
    .rodata : {
        *(.rodata)
    }
    
    /* Initialized data */
    .data : {
        *(.data)
    }
    
    /* Uninitialized data */
    .bss : {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    }
    
    /* End marker */
    __end = .;
}
