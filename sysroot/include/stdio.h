#ifndef _STDIO_H
#define _STDIO_H 1
 
#include <sys/cdefs.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
int printf(const char *, ...);
int putchar(int);
int puts(const char*);
 
#ifdef __cplusplus
}
#endif
 
#endif
