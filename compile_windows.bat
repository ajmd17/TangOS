@echo off
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/string/strcmp.c -o obj/strcmp.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/string/strlen.c -o obj/strlen.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/string/memset.c -o obj/memset.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/string/memmove.c -o obj/memmove.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/string/memcpy.c -o obj/memcpy.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/string/memcmp.c -o obj/memcmp.o -ffreestanding -O2 -Wall -Wextra

ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/stdlib/malloc.c -o obj/malloc.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/stdlib/abort.c -o obj/abort.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/stdlib/itoa.c -o obj/itoa.o -ffreestanding -O2 -Wall -Wextra

ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/stdio/printf.c -o obj/printf.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/stdio/putchar.c -o obj/putchar.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c sysroot/libc/stdio/puts.c -o obj/puts.o -ffreestanding -O2 -Wall -Wextra



ghost-i686-elf-tools\bin\i686-elf-as.exe src/asm/arch/i386/crti.s -o obj/crti.o
ghost-i686-elf-tools\bin\i686-elf-as.exe src/asm/boot.s -o obj/boot.o

ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c src/kernel/terminal.c -o obj/terminal.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c src/kernel/vgascreen.c -o obj/vgascreen.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c src/kernel/kernel.c -o obj/kernel.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c src/kernel/keyboard.c -o obj/keyboard.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c src/kernel/mouse.c -o obj/mouse.o -ffreestanding -O2 -Wall -Wextra
ghost-i686-elf-tools\bin\i686-elf-g++.exe -Isysroot/include/ -c src/kernel/power.c -o obj/power.o -ffreestanding -O2 -Wall -Wextra


ghost-i686-elf-tools\bin\i686-elf-g++.exe -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib obj/crti.o obj/boot.o obj/kernel.o obj/itoa.o obj/puts.o obj/putchar.o obj/printf.o obj/abort.o obj/memcmp.o obj/malloc.o obj/memcpy.o obj/memmove.o obj/memset.o obj/strlen.o obj/strcmp.o obj/terminal.o obj/vgascreen.o obj/keyboard.o obj/mouse.o obj/power.o -lgcc

copy "myos.bin" "isodir\boot"
copy "grub.cfg" "isodir\boot\grub"

qemu-system-i386 -kernel myos.bin
