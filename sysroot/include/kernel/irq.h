#ifndef IRQ_H
#define IRQ_H

#include <kernel/idt.h>

#define IRQ_OFF { asm volatile ("cli"); }
#define IRQ_RES { asm volatile ("sti"); }

struct regs {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

typedef struct regs regs_t;
typedef void (*irq_handler_t) (regs_t *);

static irq_handler_t irq_routines[16] = { 0 };

void irq_remap(void);
void irq_gates(void);
void irq_install(void);
void irq_ack(int irq_no);

#endif