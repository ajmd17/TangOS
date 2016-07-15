#include <kernel/irq.h>
#include <kernel/sys_asm.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();
#ifdef __cplusplus
}
#endif

void irq_install_handler(int irq, irq_handler_t handler) {
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
  irq_routines[irq] = NULL;
}

void irq_remap(void) {
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void irq_gates(void) {
	idt_set_gate(32, (unsigned)_irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)_irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)_irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)_irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)_irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)_irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)_irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)_irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)_irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)_irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)_irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)_irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)_irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)_irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned)_irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)_irq15, 0x08, 0x8E);
}

/*
 * Set up interrupt handler for hardware devices.
 */
void irq_install(void) {
	irq_remap();
	irq_gates();
	STI;
}

void irq_ack(int irq_no) {
	if (irq_no >= 12) {
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
}

#ifdef __cplusplus
extern "C"
#endif
void irq_handler(regs_t *r) {
	CLI;
  int i = r->int_no - 32;
  if (i >= 0 && i <= 15) {
    irq_handler_t handler = irq_routines[i];
    if (handler != NULL) {
      handler(r);
    } else {
      irq_ack(i);
    }
  }
	STI;
}