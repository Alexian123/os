; void ASMCALL outb(uint16_t port, uint8_t data)
global outb
outb:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

; uint8_t ASMCALL inb(uint16_t port)
global inb
inb:
    mov dx, [esp + 4]
    xor eax, eax
    mov al, [esp + 8]
    in al, dx
    ret