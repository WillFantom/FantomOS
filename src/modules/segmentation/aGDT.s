SECTION .text

GLOBAL GDT_flush
GDT_flush:
    mov eax, [esp + 4]
    lgdt [eax]
    mov ax, 0x10 ;SEG_KDATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flushjump ;SEG_KCODE
flushjump:
    ret

GLOBAL TSS_flush
TSS_flush:
    mov ax, 0x2B
    ltr ax
    ret
