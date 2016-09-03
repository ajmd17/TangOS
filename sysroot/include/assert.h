#ifndef ASSERT_H
#define ASSERT_H

#include <stddef.h>

#ifdef assert
#undef assert
#endif

#define assert(cond, str) \
  do { if (!(cond)) { abort(); } } while (0);


#endif
