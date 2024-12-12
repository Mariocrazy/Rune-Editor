; GhostC OS Bootloader
; 32-bit protected mode bootloader

MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]
[GLOBAL mboot]
[GLOBAL start]
[EXTERN main]

section .text

mboot:
    dd  MBOOT_HEADER_MAGIC
    dd  MBOOT_HEADER_FLAGS
    dd  MBOOT_CHECKSUM

start:
    push    ebx
    cli                     ; Disable interrupts
    mov     esp, stack_top  ; Set up the stack
    call    main           ; Call our main() function
    jmp     $              ; Enter an infinite loop

section .bss
    resb 8192             ; 8KB for stack
stack_top:
