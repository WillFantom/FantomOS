GLOBAL VirtMem_pagefaultAddr
VirtMem_pagefaultAddr:
    mov eax, cr2
    ret

GLOBAL VirtMem_changeDir
VirtMem_changeDir:
    mov ecx, [esp + 4]
    mov cr3, ecx
    ret
