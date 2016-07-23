#ifndef _STRING_H
#define _STRING_H 1
 
#include <sys/cdefs.h>
 
#include <stddef.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
int memcmp(const void*, const void*, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void*, const void*, size_t);
void *memset(void*, int, size_t);
size_t strlen(const char*);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *s1, const char *s2);
char *strtok(char *s, const char *delim);

#ifdef __cplusplus
}
#endif
 
#endif
