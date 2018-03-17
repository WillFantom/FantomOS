SECTION .text

GLOBAL idt_setup
idt_setup:
    mov eax, [esp + 4]
    lidt [eax]
    ret

%macro irq 2
GLOBAL irq%1
irq%1:
    cli
    push dword 0
    push dword %2
    jmp idt_common
%endmacro

irq 0, 32
irq 1, 33
irq 2, 34
irq 3, 35
irq 4, 36
irq 5, 37
irq 6, 38
irq 7, 39
irq 8, 40
irq 9, 41
irq 10, 42
irq 11, 43
irq 12, 44
irq 13, 45
irq 14, 46
irq 15, 47

;With Error Code
%macro isr_e 1
GLOBAL isr%1
isr%1:
    cli
    push dword %1
    jmp idt_common
%endmacro

isr_e 8
isr_e 10
isr_e 11
isr_e 12
isr_e 13
isr_e 14
isr_e 17

;Without Error Code
%macro isr 1
GLOBAL isr%1
isr%1:
    cli
    push dword 0
    push dword %1
    jmp idt_common
%endmacro

isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr 9
isr 15
isr 16
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr 30
isr 31
isr 128

EXTERN IDT_handle
idt_common:
    pusha
    mov eax, ds
    push eax
    mov eax, es
    push eax
    mov eax, fs
    push eax
    mov eax, gs
    push eax

    mov ax, 0x10 ; KDATA SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call IDT_handle
    add esp, 4

    mov eax, [esp + 48] ;Get the int_num from Registers (See Memory.h for the struct)
    ;THIS IS CONTEXT SWITCH STUFF
    cmp eax, 32 ;if it was a timer that cause the interrupt (like for a context switch)
    jne cont
    mov eax, DR0
    cmp eax, 0 ;esp >0 if scheduler called
    je cont
    mov esp, eax

cont:
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8 ; sort the stack out
    iret

SECTION .data

SECTION .bss
