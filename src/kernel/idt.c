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
typedef struct idt_entry idt_entry_t;

struct idt_ptr {
  unsigned short limit;
  uintptr_t base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

static struct {
  idt_entry_t entries[256];
  idt_ptr_t pointer;
} idt __attribute__((used));

#ifdef __cplusplus
extern "C" {
#endif
extern void idt_load(uintptr_t); // defined in idt_load.s
#ifdef __cplusplus
}
#endif

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
  idt.entries[num].base_low =	(base & 0xFFFF);
  idt.entries[num].base_high = (base >> 16) & 0xFFFF;
  idt.entries[num].sel = sel;
  idt.entries[num].zero =	0;
  idt.entries[num].flags = flags | 0x60;
}

void idt_install() {
  idt_ptr_t *idtp = &idt.pointer;
  idtp->limit = sizeof(idt.entries) - 1;
  idtp->base = (uintptr_t)&idt.entries[0];
  memset(&idt.entries[0], 0, sizeof idt.entries);

  idt_load((uintptr_t)idtp);
}