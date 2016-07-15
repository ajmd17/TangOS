#include <kernel/gdt.h>

// This struct defines a GDT entry. Once again we define it
// as packed to keep the structure the CPU expects the
// entry to have. We do not want the compiler to optimize
// our code!
struct gdt_entry {
  unsigned short limit_low;
  unsigned short base_low;
  byte base_middle;
  byte access;
  byte granularity;
  byte base_high;
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

// The GDT pointer defines the range of our GDT in memory.
struct gdt_ptr {
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

#ifdef __cplusplus
extern "C" {
#endif
// The GDT with three entries and a pointer to our GDT.
gdt_entry_t gdt[3];
gdt_ptr_t gdt_p;

extern void gdt_flush();

#ifdef __cplusplus
}
#endif

// Put a descriptor in the GDT
void gdt_set_gate(int num, unsigned long base, unsigned long limit, byte access, byte gran) {
  // The address of the descriptor
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;
  // Limits of the descriptor
  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);
  // Granularity and access flag
  gdt[num].granularity |= (gran & 0xF0);
  gdt[num].access = access;
}
// Setup the GDT pointer and create the three entries (gates) we
// want to use:
// 1: Null descriptor (See Tutorial)
// 2: Describes the code segment (Base address is 0, limit is 4GBytes,
// use 4 KBytes granularity, 32bit coding and a Code Segment descriptor.
// 3: Data segment which is the same as the data segment except the access
// flag which tells it is a data segment.
void gdt_install() {
  /* Setup the GDT pointer and limit */
  gdt_p.limit = (sizeof(gdt_entry_t) * 3) - 1;
  gdt_p.base = (unsigned int)&gdt; // convert pointer to unsigned int
  gdt_set_gate(0, 0, 0, 0, 0);
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
  // Flush the old GDT and install the new one. This code
  // can be found in start.asm.
  gdt_flush();
}