#ifndef SYS_ASM_H
#define SYS_ASM_H

#include <stdbool.h>
#include <stdint.h>

#define PAUSE { asm volatile ("hlt"); }
#define STOP while (true) { PAUSE; }
#define SYSCALL_VECTOR 0x7F

#define CLI { asm volatile ("cli"); }
#define STI { asm volatile ("sti"); }

static inline uint32_t farpeekl(uint16_t sel, void* off) {
    uint32_t ret;
    asm ("push %%fs\n\t"
          "mov  %1, %%fs\n\t"
          "mov  %%fs:(%2), %0\n\t"
          "pop  %%fs"
          : "=r"(ret) : "g"(sel), "r"(off));
    return ret;
}

static inline void farpokeb(uint16_t sel, void* off, uint8_t v) {
    asm ("push %%fs\n\t"
          "mov  %0, %%fs\n\t"
          "movb %2, %%fs:(%1)\n\t"
          "pop %%fs"
          : : "g"(sel), "r"(off), "r"(v));
    /* TODO: Should "memory" be in the clobber list here? */
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    /* TODO: This is probably fragile. */
    asm volatile ("jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:");
}

static inline bool are_interrupts_enabled() {
    unsigned long flags;
    asm volatile ("pushf\n\t"
                   "pop %0"
                   : "=g"(flags));
    return flags & (1 << 9);
}

static inline void lidt(void* base, uint16_t size) {   // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };
 
    asm ("lidt %0" : : "m"(IDTR));  // let the compiler choose an addressing mode
}

/* GCC has a <cpuid.h> header you should use instead of this. */
static inline void cpuid(int code, uint32_t* a, uint32_t* d) {
    asm volatile ("cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
}

static inline uint64_t rdtsc() {
    uint64_t ret;
    asm volatile ("rdtsc" : "=A"(ret));
    return ret;
}

static inline unsigned long read_cr0(void) {
    unsigned long val;
    asm volatile ("mov %%cr0, %0" : "=r"(val));
    return val;
}

static inline void invlpg(void* m) {
    /* Clobber memory to avoid optimizer re-ordering access before invlpg, which may cause nasty bugs. */
    asm volatile ("invlpg (%0)" : : "b"(m) : "memory");
}

inline void wrmsr(uint32_t msr_id, uint64_t msr_value) {
    asm volatile ("wrmsr" : : "c" (msr_id), "A" (msr_value));
}

inline uint64_t rdmsr(uint32_t msr_id) {
    uint64_t msr_value;
    asm volatile ("rdmsr" : "=A" (msr_value) : "c" (msr_id));
    return msr_value;
}
#endif
