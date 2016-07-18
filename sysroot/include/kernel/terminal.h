#ifndef TERMINAL_H
#define TERMINAL_H

#include <kernel/types.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
 
void terminal_initialize(void);
void terminal_setcolor(uint8_t);
void terminal_putchar(uchar c);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
void terminal_readstring(char *data, size_t size);
void terminal_cursorpos(int row, int col);

#endif
