; void ASMCALL idt_load(uint32_t idt_desc_ptr)
global idt_load
idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret