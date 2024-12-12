.section ".text.boot"
.global _start

// Entry point for the kernel
_start:
    // Check processor ID is zero (primary core)
    mrc p15, 0, r1, c0, c0, 5
    and r1, r1, #3
    cmp r1, #0
    bne halt

    // Setup the stack pointer
    ldr r1, =_start
    mov sp, r1

    // Clear BSS section
    ldr r4, =__bss_start
    ldr r9, =__bss_end
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b       2f

1:
    // Store multiple at r4
    stmia r4!, {r5-r8}

2:
    // If we are still below bss_end, loop
    cmp r4, r9
    blo 1b

    // Jump to our main() routine
    bl      kernel_main

    // If main returns, halt the processor
halt:
    wfe
    b       halt
