#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <stdint.h>
#include <stdio.h>
#include <kernel/sys_asm.h>

unsigned short available_ram() {
  unsigned short total;
  unsigned char lowmem, highmem;

  outb(0x70, 0x30);
  lowmem = inb(0x71);
  outb(0x70, 0x31);
  highmem = inb(0x71);

  total = lowmem | highmem << 8;
  return total;
}

#endif
