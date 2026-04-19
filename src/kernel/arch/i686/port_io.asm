[bits 32]

; void ASMCALL outb(uint16_t port, uint8_t data)
global outb
outb:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

; void ASMCALL outw(uint16_t port, uint16_t data)
global outw
outw:
    mov dx, [esp + 4]
    mov ax, [esp + 8]
    out dx, ax
    ret
    ret

; void ASMCALL outdw(uint16_t port, uint32_t data)
global outdw
outdw:
    mov dx, [esp + 4]
    mov eax, [esp + 8]
    out dx, eax
    ret
    ret

; uint8_t ASMCALL inb(uint16_t port)
global inb
inb:
    mov dx, [esp + 4]
    xor eax, eax
    mov al, [esp + 8]
    in al, dx
    ret

; uint16_t ASMCALL inw(uint16_t port)
global inw
inw:
    mov dx, [esp + 4]
    xor eax, eax
    mov ax, [esp + 8]
    in ax, dx
    ret

; uint32_t ASMCALL indw(uint16_t port)
global indw
indw:
    mov dx, [esp + 4]
    xor eax, eax
    mov eax, [esp + 8]
    in eax, dx
    ret