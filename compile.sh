#!/bin/sh

# build C standard library

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/string/strlen.c -o obj/strlen.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/string/memset.c -o obj/memset.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/string/memmove.c -o obj/memmove.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/string/memcpy.c -o obj/memcpy.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/string/memcmp.c -o obj/memcmp.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/stdlib/malloc.c -o obj/malloc.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/stdlib/abort.c -o obj/abort.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/stdlib/itoa.c -o obj/itoa.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/stdio/printf.c -o obj/printf.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/stdio/putchar.c -o obj/putchar.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c sysroot/libc/stdio/puts.c -o obj/puts.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra



# assemble crti.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/arch/i386/crti.s -o obj/crti.o

# assemble boot.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/boot.s -o obj/boot.o

#compile kernel
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c src/kernel/terminal.c -o obj/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c src/kernel/vgascreen.c -o obj/vgascreen.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Isysroot/include/ -c src/kernel/kernel.c -o obj/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#link files together
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib obj/crti.o obj/boot.o obj/kernel.o obj/itoa.o obj/puts.o obj/putchar.o obj/printf.o obj/abort.o obj/memcmp.o obj/malloc.o obj/memcpy.o obj/memmove.o obj/memset.o obj/strlen.o obj/terminal.o obj/vgascreen.o -lgcc

cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir

qemu-system-i386 myos.iso -drive file=fat:rw:./vfs/
