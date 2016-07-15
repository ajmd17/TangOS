#ifndef GDT_H
#define GDT_H

#include <kernel/types.h>

void gdt_set_gate(int num, unsigned long base, unsigned long limit, byte access, byte gran);
void gdt_install();

#endif