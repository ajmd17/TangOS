#ifndef MATH_H
#define MATH_H

#define ROUND_UP(n) \
    float __dec_n = n-(int)n; \
    if (__dec_n > 0 && __dec_n < 1) \
        n = (n-__dec_n)+1

#endif