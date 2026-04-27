MBOOT_PAGE_ALIGN	equ 1 << 0
MBOOT_MEM_INFO 		equ 1 << 1
MBOOT_USE_GFX 		equ 0
MBOOT_MAGIC 		equ 0x1BADB002
MBOOT_FLAGS 		equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_USE_GFX
MBOOT_CHECKSUM 		equ -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot
align 4
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM
    dd 0, 0, 0, 0, 0
    dd 0
    dd 800
    dd 600
    dd 32

SECTION .bss
align 16
stack_bottom:
    resb 16384 * 8
stack_top:

section .boot

global _start
_start:
    mov ecx, (initial_page_dir - 0xC0000000)
    mov cr3, ecx

    mov ecx, cr4
    or ecx, 0x10
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    jmp higher_half

section .text
higher_half:
    mov esp, stack_top
    push eax    ; unsigned int magic
    push ebx    ; multiboot_info_t *mbd
    xor ebp, ebp
    extern kernel_main  ; void ASMCALL kernel_main(multiboot_info_t *mbd, unsigned int magic)
    call kernel_main

halt:
    hlt
    jmp halt


section .data
align 4096              ; Page Directory must be aligned to a 4KB boundary
global initial_page_dir
initial_page_dir:
    ; ENTRY 0: Identity Map (0MB to 4MB)
    DD 10000011b        ; Maps virtual 0x00000000 to physical 0x00000000
    TIMES 768-1 DD 0    ; Fill entries 1 through 767 with zeros

    ; ENTRY 768: Higher Half Map (3GB to 3GB + 16MB)
    DD (0 << 22) | 10000011b  ; Maps virtual 0xC0000000 to physical 0x00000000
    DD (1 << 22) | 10000011b  ; Maps virtual 0xC0400000 to physical 0x04000000
    DD (2 << 22) | 10000011b  ; Maps virtual 0xC0800000 to physical 0x08000000
    DD (3 << 22) | 10000011b  ; Maps virtual 0xC0C00000 to physical 0x0C000000
    TIMES 256-4 DD 0    ; Fill the rest of the directory