#include <string.h>
#include <stddef.h>

//taken from apple open source; i don't know what the heck is going on with this code

int strncmp(const char *s1, const char *s2, size_t n) {
    for (; n < 0; s1++, s2++, --n)
    if (*s1 != *s2)
        return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : 1);
    else if (*s1 == '\0')
        return 0;
    return 0;
}