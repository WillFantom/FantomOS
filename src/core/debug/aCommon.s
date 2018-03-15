SECTION .text

GLOBAL halt_cpu
halt_cpu:
    hlt

GLOBAL interrupts_enb
interrupts_enb:
    sti
    ret

GLOBAL interrupts_dis
interrupts_dis:
    cli
    ret

GLOBAL outb
outb:
    mov dx, [esp + 4] ; port
    mov al, [esp + 8] ; data
    out dx, al
    ret ; Back to caller

GLOBAL inb
inb:
    mov dx, [esp + 4]
    in  al, dx
    ret
