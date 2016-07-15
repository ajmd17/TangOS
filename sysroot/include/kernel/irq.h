#ifndef IRQ_H
#define IRQ_H

#include <kernel/idt.h>
#include <stddef.h>

typedef struct {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
} regs_t;

typedef void (*irq_handler_t) (regs_t *);
static irq_handler_t irq_routines[16] = { NULL };

void irq_install_handler(int irq, irq_handler_t handler);
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_gates(void);
void irq_install(void);
void irq_ack(int irq_no);

#endif