#!/bin/sh

# build C standard library

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/strcpy.c -o obj/strcpy.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/strcmp.c -o obj/strcmp.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/strlen.c -o obj/strlen.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/memset.c -o obj/memset.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/memmove.c -o obj/memmove.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/memcpy.c -o obj/memcpy.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/string/memcmp.c -o obj/memcmp.o -ffreestanding -O2 -w

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdlib/malloc.c -o obj/malloc.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdlib/abort.c -o obj/abort.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdlib/itoa.c -o obj/itoa.o -ffreestanding -O2 -w

./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdio/printf.c -o obj/printf.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdio/putchar.c -o obj/putchar.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c sysroot/libc/stdio/puts.c -o obj/puts.o -ffreestanding -O2 -w



# assemble crti.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/arch/i386/crti.s -o obj/crti.o

# assemble gdt_flush.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/gdt_flush.s -o obj/gdt_flush.o

# assemble idt_load.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/idt_load.s -o obj/idt_load.o

# assemble isr_load.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/isr_load.s -o obj/isr_load.o

# assemble irq_handle.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/irq_handle.s -o obj/irq_handle.o

# assemble boot.s
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-as src/asm/boot.s -o obj/boot.o

#compile kernel
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/terminal.c -o obj/terminal.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/vgascreen.c -o obj/vgascreen.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/kernel.c -o obj/kernel.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/keyboard.c -o obj/keyboard.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/mouse.c -o obj/mouse.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/power.c -o obj/power.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/idt.c -o obj/idt.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/irq.c -o obj/irq.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/gdt.c -o obj/gdt.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/isrs.c -o obj/isrs.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/kernel/image.c -o obj/image.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/gui/font.c -o obj/font.o -ffreestanding -O2 -w
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -Isysroot/include/ -c src/gui/button.c -o obj/button.o -ffreestanding -O2 -w


#link files together
./i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-g++ -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib obj/crti.o obj/gdt_flush.o obj/isr_load.o obj/idt_load.o obj/irq_handle.o obj/boot.o obj/kernel.o obj/itoa.o obj/puts.o obj/putchar.o obj/printf.o obj/abort.o obj/memcmp.o obj/malloc.o obj/memcpy.o obj/memmove.o obj/memset.o obj/strcpy.o obj/strlen.o obj/strcmp.o obj/terminal.o obj/vgascreen.o obj/button.o obj/keyboard.o obj/mouse.o obj/power.o obj/idt.o obj/irq.o obj/gdt.o obj/isrs.o obj/image.o obj/font.o -lgcc

	qemu-system-i386 -kernel myos.bin

