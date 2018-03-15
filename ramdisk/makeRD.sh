
echo "Making Static Library"

i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o syscall.o -c libs/syscall.c
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o malloc.o -c libs/malloc.c
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o stdio.o -c libs/stdio.c
#i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ -o file.o -c libs/file.c

ar rcs lib.a syscall.o malloc.o stdio.o

echo "Making UserSpace Apps"
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o shell.o -c apps/shell.c
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o test.o -c apps/test.c

ld -T linker.ld -m elf_i386 shell.o lib.a -o shell 
ld -T linker.ld -m elf_i386 test.o lib.a -o test 

tar -cf ramdisk.tar shell test img.ascii

cp ramdisk.tar ./../build/iso/boot/
