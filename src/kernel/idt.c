#include <kernel/idt.h>
#include <string.h>
#include <stdint.h>

struct idt_entry {
  unsigned short base_low;
  unsigned short sel;
  unsigned char zero;
  unsigned char flags;
  unsigned short base_high;
} __attribute__((packed));

struct idt_ptr {
  unsigned short limit;
  uintptr_t base;
} __attribute__((packed));

typedef struct idt_entry idt_entry_t;
typedef struct idt_ptr idt_ptr_t;

idt_entry_t idt[256];
idt_ptr_t idtp;

#ifdef __cplusplus
extern "C" {
#endif
extern void idt_load(); // defined in idt_load.s
#ifdef __cplusplus
}
#endif

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
  idt[num].base_low =	(base & 0xFFFF);
  idt[num].base_high = (base >> 16) & 0xFFFF;
  idt[num].sel = sel;
  idt[num].zero =	0;
  idt[num].flags = flags | 0x60;
}

void idt_install() {
  idtp.limit = (sizeof(idt_entry_t) * 256) - 1;
  idtp.base = (uintptr_t)&idt;
  memset(&idt, 0, sizeof(idt_entry_t) * 256);

  idt_load();
}