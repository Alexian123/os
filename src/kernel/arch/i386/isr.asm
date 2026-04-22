extern isr_handler_common

%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push dword 0        ; dummy error code
    push dword %1       ; interrupt number
    jmp isr_common
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    push dword %1       ; interrupt number
    jmp isr_common
%endmacro

%include "arch/i386/isr_stubs.inc"

isr_common:
    pusha

    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp        ; pass registers_t*

    call isr_handler_common

    add esp, 4      ; remove argument

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa

    add esp, 8      ; pop int_no + err_code

    ;sti
    iret