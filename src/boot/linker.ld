ENTRY(_start)

MEMORY {
    ram : ORIGIN = 0x100000, LENGTH = 128M
}

SECTIONS {
    . = 0x100000;

    .multiboot ALIGN(4K) : {
        KEEP(*(.multiboot))
    } > ram

    .text ALIGN(4K) : {
        *(.text)
        *(.text.*)
        *(.gnu.linkonce.t.*)
    } > ram

    .rodata ALIGN(4K) : {
        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r.*)
    } > ram

    .data ALIGN(4K) : {
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
    } > ram

    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    } > ram

    /DISCARD/ : {
        *(.comment)
        *(.note.gnu.build-id)
    }
}
