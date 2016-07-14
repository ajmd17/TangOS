#ifndef IDT_H
#define IDT_H

void idt_install();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

#endif