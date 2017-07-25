#ifndef ASSERT_H
#define ASSERT_H

#include <stddef.h>
#include <stdio.h>

#ifdef assert
#undef assert
#endif

#define assert(cond, str) \
  do { if (!(cond)) { printf(str); abort(); } } while (0);


#endif
