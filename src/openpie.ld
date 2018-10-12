/*
    GNU linker script for OPENPIE
*/

/* Specify the memory areas */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (rw)       : ORIGIN = 0x20000000, LENGTH = 64K
    RAM (rw)        : ORIGIN = 0x60000000, LENGTH = 192K
}

/* produce a link error if there is not this amount of SRAM/RAM for these sections */
_minimum_stack_size = 16K;
_minimum_heap_size = 128K;

/* Define tho top end of the stack.  The stack is full descending so begins just
   above last byte of SRAM.  Note that EABI requires the stack to be 8-byte
   aligned for a call. */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

/* RAM extents for the garbage collector */
_sram_start = ORIGIN(SRAM);
_sram_end = ORIGIN(SRAM) + LENGTH(SRAM);
_ram_start = ORIGIN(RAM); /* heap starts just after statically allocated memory */
_ram_end = ORIGIN(RAM) + LENGTH(RAM);

/* Memory layout for basic configuration:

    FLASH       .startup
    FLASH       .text
    FLASH       .data

    SRAM         .data
    SRAM         .bss
    SRAM         .stack

    RAM         .heap
*/

ENTRY(Reset_Handler)

/* define output sections */
SECTIONS
{
    /* The startup code goes first into FLASH */
    .startup :
    {
        . = ALIGN(4);
        _svec = .;
        KEEP(*(.startup)) /* Startup code */

        . = ALIGN(4);
    } >FLASH

    /* The program code and other data goes into FLASH */
    .text :
    {
        . = ALIGN(4);
        *(.text*)          /* .text* sections (code) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    /*  *(.glue_7)   */    /* glue arm to thumb code */
    /*  *(.glue_7t)  */    /* glue thumb to arm code */

        . = ALIGN(4);
        _etext = .;        /* define a global symbol at end of code */
    } >FLASH

    /* used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /* This is the initialized data section
    The program executes knowing that the data is in the SRAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to SRAM. */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* create a global symbol at data start; used by startup code in order to initialise the .data section in SRAM */
        *(.data*)          /* .data* sections */

        . = ALIGN(4);
        _edata = .;        /* define a global symbol at data end; used by startup code in order to initialise the .data section in SRAM */
    } >SRAM AT> FLASH

    /* Uninitialized data section */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* define a global symbol at bss start; used by startup code */
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;         /* define a global symbol at bss end; used by startup code and GC */
    } >SRAM

    /* this just checks there is enough SRAM for the stack */
    .stack :
    {
        . = ALIGN(4);
        . = . + _minimum_stack_size;
        . = ALIGN(4);
    } >SRAM

    /* this is to define the start of the heap, and make sure we have a minimum size */
    .heap :
    {
        . = ALIGN(4);
        . = . + _minimum_heap_size;
        . = ALIGN(4);
    } >RAM

    .ARM.attributes 0 : { *(.ARM.attributes) }
}
