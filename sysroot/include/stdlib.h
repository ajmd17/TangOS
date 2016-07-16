#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
#endif
__attribute__((__noreturn__)) void abort(void);

char *itoa(int value, char *str, int base); // converts integer to string
char *utoa(unsigned value, char *str, int base); // converts unsigned integer to string
void free(void*);
void *malloc(size_t);
void *calloc(size_t);
void *realloc(void *blk, size_t size);
void dump_heap(void);

#endif
