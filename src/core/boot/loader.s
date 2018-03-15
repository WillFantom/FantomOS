; Enables 4MB Pages via PSE bit and maps the kernel space (first 4MB of RAM) to
; the load point 3GB into the virtual address space
;
; Will Fantom

EXTERN kmain
EXTERN k_mem_end ; For Memory Mgmt
GLOBAL Initial_Page_Dir

; Multiboot Stuff
MB_MAGIC equ 0x1BADB002     ; MAGIC NUMBER
MB_PA equ 1<<0              ; PAGE ALIGN
MB_MI equ 1<<1              ; MEMORY INFO
MB_FLAGS equ MB_PA | MB_MI
MB_CHECK equ -(MB_MAGIC + MB_FLAGS)

; Virtual Memory Addressing
K_VMA equ 0xC0000000    ; 3GB In
K_PN equ (K_VMA >> 22)  ; 768 for 3GB in

SECTION .data
align 4096
; Make the initial page dir
; Easier due to the 4MB pages rather than 4KB
; Only 1 kernel page (K_PN = 768 @ 3GB)
Initial_Page_Dir:
    dd 0x00000083
    times (K_PN - 1) dd 0 ; empty pages pre-kernel
    dd 0x00000083
    times (1024 - K_PN - 1) dd 0 ; empty pages post-kernel

SECTION .text
align 4

EXTERN text
EXTERN bssstart
EXTERN bssend
GLOBAL mbh

mbh:
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECK
    dd mbh
    dd text
    dd bssstart
    dd bssend
    dd load


; Stack Stuff
STACK_SIZE equ 0x4000 ; 16kb

GLOBAL load

load:
    mov ecx, (Initial_Page_Dir - K_VMA)
    mov cr3, ecx

    mov ecx, cr4
    or ecx, 0x00000010 ; BIT 7 - is 4MB PAGE
    mov cr4, ecx

    mov ecx, cr0
    or ecx, 0x80000000 ; Enable Paging
    mov cr0, ecx

    ; Move to Kernel Space va
    lea ecx, [start]
    jmp ecx

start:
    ; Unmapping
    mov dword [Initial_Page_Dir], 0
    invlpg [0]
    cli
    mov esp, stackSpace+STACK_SIZE
    add ebx, K_VMA ; As not in first 4mb need to relocate the mb_info pointer
    push eax
    push k_mem_end
    push ebx

    call kmain ; Finally See
    hlt

SECTION .bss
align 32
stackSpace:
    resb STACK_SIZE
