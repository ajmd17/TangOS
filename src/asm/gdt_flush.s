.global gdt_flush
.extern gdt_p
.type gdt_flush @function
gdt_flush:
   lgdt (gdt_p)
   mov $0x10, %ax
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   mov %ax, %ss
   ljmp $0x08, $flush2
flush2:
   ret
