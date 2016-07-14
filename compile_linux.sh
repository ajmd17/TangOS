#!/bin/sh

# build C standard library

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/strcmp.c -o obj/strcmp.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/strlen.c -o obj/strlen.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/memset.c -o obj/memset.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/memmove.c -o obj/memmove.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/memcpy.c -o obj/memcpy.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/memcmp.c -o obj/memcmp.o -ffreestanding -O2 -Wall -Wextra

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdlib/malloc.c -o obj/malloc.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdlib/abort.c -o obj/abort.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdlib/itoa.c -o obj/itoa.o -ffreestanding -O2 -Wall -Wextra

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdio/printf.c -o obj/printf.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdio/putchar.c -o obj/putchar.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdio/puts.c -o obj/puts.o -ffreestanding -O2 -Wall -Wextra



# assemble crti.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/arch/i386/crti.s -o obj/crti.o

# assemble idt_load.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/idt_load.s -o obj/idt_load.o

# assemble boot.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/boot.s -o obj/boot.o

#compile kernel
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/terminal.c -o obj/terminal.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/vgascreen.c -o obj/vgascreen.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/kernel.c -o obj/kernel.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/keyboard.c -o obj/keyboard.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/mouse.c -o obj/mouse.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/power.c -o obj/power.o -ffreestanding -O2 -Wall -Wextra
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/idt.c -o obj/idt.o -ffreestanding -O2 -Wall -Wextra


#link files together
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib obj/crti.o obj/idt_load.o obj/boot.o obj/kernel.o obj/itoa.o obj/puts.o obj/putchar.o obj/printf.o obj/abort.o obj/memcmp.o obj/malloc.o obj/memcpy.o obj/memmove.o obj/memset.o obj/strlen.o obj/strcmp.o obj/terminal.o obj/vgascreen.o obj/keyboard.o obj/mouse.o obj/power.o obj/idt.o -lgcc

cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg

#create an iso
if grub-mkrescue -o myos.iso isodir ; then
	qemu-system-i386 myos.iso -drive file=fat:rw:./vfs/
else
  echo "Error while creating iso image"
fi
