[org 0x7C00]

%define CR 0x0D
%define LF 0x0A
%define ENDL CR, LF

start:
    xor ax, ax
    mov ds, ax

    mov si, msg
    call puts

    cli
halt:
    jmp halt

;
; Prints string from DS:SI using BIOS
;
puts:
    cld         ; needed for lodsb
.putc_loop:
    lodsb
    or al, al
    jz .putc_end
    mov ah, 0x0E
    mov bh, 0
    int 0x10    ; call BIOS interrupt 0x10 with AH=0x0E
    jmp .putc_loop
.putc_end:
    ret

msg: db 'Hello Bootloader', ENDL, 0 

times 510-($-$$) db 0
db 0x55
db 0xAA