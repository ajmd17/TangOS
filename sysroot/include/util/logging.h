#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#define ENABLE_DEBUG_PRINT 0

#if ENABLE_DEBUG_PRINT
#define debug_print(level, fmt, ...) \
  printf(#level ": " fmt, __VA_ARGS__)

#else 
#define debug_print(level, fmt, ...) \
  (void*)0;
#endif

#endif