#ifndef _STDIO_H
#define _STDIO_H 1
 
#include <sys/cdefs.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
int printf(const char *, ...);
int putchar(int);
int puts(const char*);

typedef struct {
    int id;
} FILE;

FILE *fopen(const char *path, const char *mode);
int fclose(FILE *stream);
int fprintf(FILE* file, const char *, ...);
 
#ifdef __cplusplus
}
#endif
 
#endif
