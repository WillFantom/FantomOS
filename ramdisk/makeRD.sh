
echo "Making Static Library"

i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o syscall.o -c libs/syscall.c
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o malloc.o -c libs/malloc.c
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o stdio.o -c libs/stdio.c
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o string.o -c libs/string.c
#i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ -o file.o -c libs/file.c

ar rcs lib.a syscall.o malloc.o stdio.o string.o

echo "Making UserSpace Apps"
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o shell.o -c apps/shell.c
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o test.o -c apps/test.c
i686-elf-gcc -fno-stack-protector -w -std=c99 -fno-builtin -m32 -I libs/ apps/ -o echo.o -c apps/echo.c

ld -T linker.ld -m elf_i386 shell.o lib.a -o shell 
ld -T linker.ld -m elf_i386 test.o lib.a -o test
ld -T linker.ld -m elf_i386 echo.o lib.a -o echo  

tar -cf ramdisk.tar shell test echo img.ascii

cp ramdisk.tar ./../build/iso/boot/
