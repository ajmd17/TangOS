#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#define ENABLE_DEBUG_PRINT 1

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

#if ENABLE_DEBUG_PRINT
#define debug_print(level, fmt, ...) \
  printf("(" #level ", " __FILE__ ", " S__LINE__ "): " fmt "\n", __VA_ARGS__)

#else 
#define debug_print(level, fmt, ...) \
  (void*)0;
#endif

#endif