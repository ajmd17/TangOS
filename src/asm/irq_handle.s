.section .text
.align 4
  
.extern irq_handler

# irq stuff
.global _irq0
.global _irq1
.global _irq2
.global _irq3
.global _irq4
.global _irq5
.global _irq6
.global _irq7
.global _irq8
.global _irq9
.global _irq10
.global _irq11
.global _irq12
.global _irq13
.global _irq14
.global _irq15

# 32: IRQ0
_irq0:
  cli
  pushl $0
  pushl $32
  jmp irq_common_stub

# 33: IRQ1
_irq1:
  cli
  pushl $0
  pushl $33
  jmp irq_common_stub
  
# 34: IRQ2
_irq2:
  cli
  pushl $0
  pushl $34
  jmp irq_common_stub
  
# 35: IRQ3
_irq3:
  cli
  pushl $0
  pushl $35
  jmp irq_common_stub
  
# 36: IRQ4
_irq4:
  cli
  pushl $0
  pushl $36
  jmp irq_common_stub
  
# 37: IRQ5
_irq5:
  cli
  pushl $0
  pushl $37
  jmp irq_common_stub
  
# 38: IRQ6
_irq6:
  cli
  pushl $0
  pushl $38
  jmp irq_common_stub
  
# 39: IRQ7
_irq7:
  cli
  pushl $0
  pushl $39
  jmp irq_common_stub
  
# 40: IRQ8
_irq8:
  cli
  pushl $0
  pushl $40
  jmp irq_common_stub
  
# 41: IRQ9
_irq9:
  cli
  pushl $0
  pushl $41
  jmp irq_common_stub
  
# 42: IRQ10
_irq10:
  cli
  pushl $0
  pushl $42
  jmp irq_common_stub
  
# 43: IRQ11
_irq11:
  cli
  pushl $0
  pushl $43
  jmp irq_common_stub
  
# 44: IRQ12
_irq12:
  cli
  pushl $0
  pushl $44
  jmp irq_common_stub
  
# 45: IRQ13
_irq13:
  cli
  pushl $0
  pushl $45
  jmp irq_common_stub

# 46: IRQ14
_irq14:
  cli
  pushl $0
  pushl $46
  jmp irq_common_stub
  
# 47: IRQ15
_irq15:
  cli
  pushl $0
  pushl $47
  jmp irq_common_stub

.type irq_handler, @function

irq_common_stub:
  /* Save all registers */
  pusha

  /* Save segment registers */
  push %ds
  push %es
  push %fs
  push %gs
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  /* Call interrupt handler */
  push %esp
  call irq_handler
  add $4, %esp

  /* Restore segment registers */
  pop %gs
  pop %fs
  pop %es
  pop %ds

  /* Restore all registers */
  popa
  /* Cleanup error code and IRQ number */
  add $8, %esp
  /* pop CS, EIP, EFLAGS, SS and ESP */
  iret
