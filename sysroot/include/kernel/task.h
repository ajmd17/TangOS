#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef struct page {
  unsigned int present:1;
  unsigned int rw:1;
  unsigned int user:1;
  unsigned int accessed:1;
  unsigned int dirty:1;
  unsigned int unused:7;
  unsigned int frame:20;
} __attribute__((packed)) page_t;

typedef struct page_table {
  page_t pages[1024];
} page_table_t;

typedef struct page_directory {
  uintptr_t physical_tables[1024];  /* Physical addresses of the tables */
  page_table_t *tables[1024]; /* 1024 pointers to page tables... */
  uintptr_t physical_address; /* The physical address of physical_tables */
  int32_t ref_count;
} page_directory_t;


extern uintptr_t read_eip(void);
extern void copy_page_physical(uint32_t, uint32_t);
extern page_directory_t * clone_directory(page_directory_t * src);
extern page_table_t * clone_table(page_table_t * src, uintptr_t * physAddr);
extern void move_stack(void *new_stack_start, uint32_t size);
extern void kexit(int retval);
extern void task_exit(int retval);
extern uint32_t next_pid;

extern void tasking_install(void);
extern void switch_task(uint8_t reschedule);
extern void switch_next(void);
extern uint32_t fork(void);
extern uint32_t clone(uintptr_t new_stack, uintptr_t thread_func, uintptr_t arg);
extern uint32_t getpid(void);
extern void enter_user_jmp(uintptr_t location, int argc, char ** argv, uintptr_t stack);

#endif