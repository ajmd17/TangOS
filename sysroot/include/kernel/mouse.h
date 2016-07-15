#ifndef MOUSE_H
#define MOUSE_H

#include <kernel/irq.h>

extern int mouse_dx;
extern int mouse_dy;

void mouse_handler(regs_t *regs);
void mouse_install(void);

#endif
