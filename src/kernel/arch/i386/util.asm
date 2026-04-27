global flush_tlb
flush_tlb:
    mov eax, cr3
    mov cr3, eax
    ret